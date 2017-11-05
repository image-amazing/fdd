#include"FaceAnalysisModel.h"
#include"Face.h"
#include<exception>
FaceAnalysisModel::FaceAnalysisModel()
	:faceDetectionTime_(0),faceAlignmentTime_(0)
{

}

FaceAnalysisModel::FaceAnalysisModel(const std::string &fccPath,const std::string &pfccPath,const std::string &rightEyeStatusSVMPath
    ,const std::string &mouthChinStatusSVMPath,const std::string &featurePointsRegressorPath,const std::string &regPath
	,const std::string &rightEyeDNNModelPath,const std::string &rightEyeDNNWeightsPath
	,const std::string &rightEyeDNNMeanPath,const std::string &rightEyeDNNLabelPath
	,const std::string &mouthChinDNNModelPath,const std::string &mouthChinDNNWeightsPath
	,const std::string &mouthChinDNNMeanPath,const std::string &mouthChinDNNLabelPath
	,const std::string &leftEyeDNNModelPath,const std::string &leftEyeDNNWeightsPath
    ,const std::string &leftEyeDNNMeanPath,const std::string &leftEyeDNNLabelPath
    ,const std::string &headposeDNNModelPath,const std::string &headposeDNNWeightsPath
    ,const std::string &headposeDNNMeanPath,const std::string &headposeDNNLabelPath )
	:faceDetectionTime_(0),faceAlignmentTime_(0)
{
	fcc_.load(fccPath);
    pfcc_.load(pfccPath);
	rightEyeStatusSVM_ = cv::ml::SVM::load(rightEyeStatusSVMPath);
	mouthChinStatusSVM_ = cv::ml::SVM::load(mouthChinStatusSVMPath);
    featurePointsRegressor_.Load(featurePointsRegressorPath,regPath);
	rightEyeStatusDNN_.reset(new CaffeClassifier(rightEyeDNNModelPath,rightEyeDNNWeightsPath,rightEyeDNNMeanPath,rightEyeDNNLabelPath));
	leftEyeStatusDNN_.reset(new CaffeClassifier(leftEyeDNNModelPath, leftEyeDNNWeightsPath, leftEyeDNNMeanPath, leftEyeDNNLabelPath));
	mouthChinStatusDNN_.reset(new CaffeClassifier(mouthChinDNNModelPath,mouthChinDNNWeightsPath,mouthChinDNNMeanPath,mouthChinDNNLabelPath));
    headposeDNN_.reset(new CaffeClassifier(headposeDNNModelPath,headposeDNNWeightsPath,headposeDNNMeanPath,headposeDNNLabelPath));
}

FaceAnalysisModel::~FaceAnalysisModel()
{
	
}

void FaceAnalysisModel::fcc(const std::string &fccPath)
{
	fcc_.load(fccPath);
}

void FaceAnalysisModel::rightEyeStatusSVM(const std::string &rightEyeStatusSVMPath)
{
	rightEyeStatusSVM_ = cv::ml::SVM::load(rightEyeStatusSVMPath);
}

void FaceAnalysisModel::mouthChinStatusSVM(const std::string &mouthChinStatusSVMPath)
{
	mouthChinStatusSVM_ = cv::ml::SVM::load(mouthChinStatusSVMPath);
}

void FaceAnalysisModel::featurePointsRegressor(const std::string &featurePointsRegressorPath,const std::string &regPath)
{
    featurePointsRegressor_.Load(featurePointsRegressorPath,regPath);
}

void FaceAnalysisModel::detectFaces(const cv::Mat &inputImg, std::vector<cv::Rect> &faces,FaceAnalysisModel::FaceType faceType ,double scale ,
	int minNeighbor, int flags, cv::Size minSize , cv::Size maxSize)
{
	if (3==inputImg.channels())
	{
		cv::cvtColor(inputImg,inputImg,CV_BGR2GRAY);
	}
    cv::CascadeClassifier fcc;
    if(FaceAnalysisModel::FaceType::Front==faceType){
        fcc=fcc_;
    }else if(FaceAnalysisModel::FaceType::Left==faceType){
        fcc=pfcc_;
    }else if(FaceAnalysisModel::FaceType::Right==faceType){
        fcc=pfcc_;
         cv::flip(inputImg,inputImg,1);
    }else{
        throw std::exception();
    }
	clock_t begin = clock();
    fcc.detectMultiScale(inputImg,faces,scale,minNeighbor,flags,minSize,maxSize);
	clock_t end = clock();
	faceDetectionTime_ = double(end - begin) / CLOCKS_PER_SEC;
    if(FaceAnalysisModel::FaceType::Right==faceType){
        cv::flip(inputImg,inputImg,1);
    }
}

inline BoundingBox FaceAnalysisModel::convertRectToBoundingBox(const cv::Rect &rect)
{
	BoundingBox boundingbox;
	boundingbox.start_x = rect.x;
	boundingbox.start_y = rect.y;
	boundingbox.width = (rect.width - 1);
	boundingbox.height = (rect.height - 1);
	boundingbox.centroid_x = boundingbox.start_x + boundingbox.width / 2.0;
	boundingbox.centroid_y = boundingbox.start_y + boundingbox.height / 2.0;
	return boundingbox;
}

void FaceAnalysisModel::regressFeaturePoints(const cv::Mat &inputImg, const cv::Rect &face,
	std::vector<cv::Point> &featurePoints ,cv::Mat_<double> &regressedShapeWithoutScale , double scale)
{
	featurePoints.clear();
	if (3==inputImg.channels())
	{
		cv::cvtColor(inputImg,inputImg,CV_BGR2GRAY);
	}
    //face area to compute feature points
	BoundingBox boundingbox = convertRectToBoundingBox(face);
	clock_t begin = clock();
    regressedShapeWithoutScale = featurePointsRegressor_.Predict(inputImg, boundingbox, 0);
	clock_t end = clock();
	faceAlignmentTime_ = double(end - begin) / CLOCKS_PER_SEC;
	for (int i = 0;i < global_params.landmark_num;i++) {
		//circle(cImg, Point2d(current_shape(i, 0)/ scaleForFaceDetection, current_shape(i, 1)/ scaleForFaceDetection), 2, Scalar(255, 255, 255), -1, 8, 0);
        //save all feature points
		featurePoints.push_back(cv::Point(regressedShapeWithoutScale(i, 0)/scale , regressedShapeWithoutScale(i, 1)/scale));
		/*char buf[8];
		sprintf(buf, "%d", i);
		putText(cImg, buf, cv::Point(current_shape(i, 0) * 5, current_shape(i, 1) * 5), 0, 0.5, cv::Scalar(255, 0, 0));*/
	}
}

FaceComponent::Status FaceAnalysisModel::predictRightEyeStatus(cv::Mat rightEyeImg,const cv::Size &normalizedSize)
{
	if (3==rightEyeImg.channels())
	{
		cv::cvtColor(rightEyeImg,rightEyeImg,CV_BGR2GRAY);
	}
	cv::resize(rightEyeImg,rightEyeImg,normalizedSize);
	cv::equalizeHist(rightEyeImg,rightEyeImg);
	cv::HOGDescriptor eyeHog(normalizedSize, cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);
	std::vector<float>eyeHogDescriptor;
	eyeHog.compute(rightEyeImg, eyeHogDescriptor);
	cv::Mat eyeHogFeature(1, eyeHog.getDescriptorSize(), CV_32FC1);
	for (size_t i = 0;i<eyeHogDescriptor.size();i++)
	{
		eyeHogFeature.at<float>(0, i) = eyeHogDescriptor[i];
	}
    //predict eye status
	float rightEyeLabel = rightEyeStatusSVM_->predict(eyeHogFeature);
	return static_cast<FaceComponent::Status>(static_cast<int>(rightEyeLabel));
}

FaceComponent::Status FaceAnalysisModel::predictRightEyeStatusByDNN(cv::Mat rightEyeImg,const cv::Size &normalizedSize) {
    return static_cast<FaceComponent::Status>(predictStatusByDNN(rightEyeImg,normalizedSize,rightEyeStatusDNN_));
}
FaceComponent::Status FaceAnalysisModel::predictLeftEyeStatusByDNN(cv::Mat leftEyeImg,const cv::Size &normalizedSize) {
    return static_cast<FaceComponent::Status>(predictStatusByDNN(leftEyeImg,normalizedSize,leftEyeStatusDNN_));
}

FaceComponent::Status FaceAnalysisModel::predictMouthStatus(cv::Mat mouthImg,const cv::Size &normalizedSize)
{
	if (3==mouthImg.channels())
	{
		cv::cvtColor(mouthImg,mouthImg,CV_BGR2GRAY);
	}
	cv::resize(mouthImg, mouthImg , normalizedSize);
	cv::equalizeHist(mouthImg, mouthImg);
	cv::HOGDescriptor mouthHog(normalizedSize, cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);
	std::vector<float> mouthHogDescriptor;
	mouthHog.compute(mouthImg, mouthHogDescriptor);
	cv::Mat mouthHogFeature(1, mouthHog.getDescriptorSize(), CV_32FC1);
	for (size_t i = 0;i<mouthHogDescriptor.size();i++)
	{
		mouthHogFeature.at<float>(0, i) = mouthHogDescriptor[i];
	}
    //predict eye status
	float mouthLabel = mouthChinStatusSVM_->predict(mouthHogFeature);
	return static_cast<FaceComponent::Status>(static_cast<int>(mouthLabel));
}

FaceComponent::Status FaceAnalysisModel::predictMouthStatusByDNN(cv::Mat mouthChinImg, const cv::Size &normalizedSize) {
    return static_cast<FaceComponent::Status>(predictStatusByDNN(mouthChinImg,normalizedSize,mouthChinStatusDNN_));
}

int FaceAnalysisModel::predictHeadposeByDNN(cv::Mat faceImg,const cv::Size &normalizedSize){
    return predictStatusByDNN(faceImg,normalizedSize,headposeDNN_);
}

double FaceAnalysisModel::faceDetectionTime()
{
	return faceDetectionTime_;
}

double FaceAnalysisModel::faceAlignmentTime()
{
	return faceAlignmentTime_;
}

cv::Mat_<double> FaceAnalysisModel::meanFeaturePointsShape()
{
	return featurePointsRegressor_.mean_shape_;
}
