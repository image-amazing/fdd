#pragma once
#include<opencv2/opencv.hpp>
#include"LBFRegressor.h"
#include"FaceComponent.h"
#include"CaffeClassifier.h"
namespace fdd{
class Face;
class FaceAnalysisModel {
private:
	cv::CascadeClassifier fcc_;
    cv::CascadeClassifier pfcc_;
	cv::Ptr<cv::ml::SVM> rightEyeStatusSVM_;
	cv::Ptr<CaffeClassifier> rightEyeStatusDNN_;
    cv::Ptr<cv::ml::SVM> leftEyeStatusSVM_;
	cv::Ptr<CaffeClassifier> leftEyeStatusDNN_;
	cv::Ptr <cv::ml::SVM> mouthChinStatusSVM_;
	cv::Ptr<CaffeClassifier> mouthChinStatusDNN_;
    cv::Ptr<CaffeClassifier> headposeDNN_;
    LBF::LBFRegressor featurePointsRegressor_;
	double faceDetectionTime_;
	double faceAlignmentTime_;
public:
    enum FaceType{Front,Left,Right};
public:
	FaceAnalysisModel();
    FaceAnalysisModel(const std::string &fccPath,const std::string &pfccPath,const std::string &rightEyeStatusSVMPath
        ,const std::string &mouthChinStatusSVMPath,const std::string &featurePointsRegressorPath,const std::string &regPath
		,const std::string &rightEyeDNNModelPath,const std::string &rightEyeDNNWeightsPath
		,const std::string &rightEyeDNNMeanPath,const std::string &rightEyeDNNLabelPath
		,const std::string &mouthChinDNNModelPath,const std::string &mouthChinDNNWeightsPath
		,const std::string &mouthChinDNNMeanPath,const std::string &mouthChinDNNLabelPath
		,const std::string &leftEyeDNNModelPath,const std::string &leftEyeDNNWeightsPath
        ,const std::string &leftEyeDNNMeanPath,const std::string &leftEyeDNNLabelPath
        ,const std::string &headposeDNNModelPath,const std::string &headposeDNNWeightsPath
        ,const std::string &headposeDNNMeanPath,const std::string &headposeDNNLabelPath);
	~FaceAnalysisModel();
    void loadFCC(const std::string &fccPath);
    void loadPFCC(const std::string &pfccPath);
    void loadRightEyeStatusSVM(const std::string &rightEyeStatusSVMPath);
    void loadRightEyeStatusDNN(const std::string &rightEyeDNNModelPath
                                ,const std::string &rightEyeDNNWeightsPath
                                ,const std::string &rightEyeDNNMeanPath
                                ,const std::string &rightEyeDNNLabelPath);
    void loadLeftEyeStatusSVM(const std::string &leftEyeStatusSVMPath);
    void loadLeftEyeStatusDNN(const std::string &leftEyeDNNModelPath
                               ,const std::string &leftEyeDNNWeightsPath
                               ,const std::string &leftEyeDNNMeanPath
                               ,const std::string &leftEyeDNNLabelPath);
    void loadMouthChinStatusSVM(const std::string &mouthChinStatusSVMPath);
    void loadMouthChinStatusDNN(const std::string &mouthChinDNNModelPath,const std::string &mouthChinDNNWeightsPath
                                ,const std::string &mouthChinDNNMeanPath,const std::string &mouthChinDNNLabelPath);
    void loadHeadposeDNN(const std::string &headposeDNNModelPath,const std::string &headposeDNNWeightsPath
                         ,const std::string &headposeDNNMeanPath,const std::string &headposeDNNLabelPath);
    void loadFeaturePointsRegressor(const std::string &featurePointsRegressor,const std::string &regPath);
    void detectFaces(const cv::Mat &inputImg,std::vector<cv::Rect> &faces,FaceAnalysisModel::FaceType faceType,double scale=1.1,
		int minNeighbor=3,int flags=0,cv::Size minSize=cv::Size(),cv::Size maxSize=cv::Size());
    void regressFeaturePoints(const cv::Mat &inputImg,const cv::Rect &face ,std::vector<cv::Point> &featurePoints
                              , cv::Mat_<double> &regressedShapeWithoutScale,double scale=1.0);
    FaceComponent::Status predictRightEyeStatus(cv::Mat rightEyeImg,const cv::Size &normalizedSize=cv::Size(48,16));
    FaceComponent::Status predictRightEyeStatusByDNN(cv::Mat rightEyeImg,const cv::Size &normalizedSize=cv::Size(48,24));
    FaceComponent::Status predictLeftEyeStatusByDNN(cv::Mat cImg,const cv::Size &normalizedSize = cv::Size(48, 24));
    FaceComponent::Status predictMouthStatus(cv::Mat mouthImg,const cv::Size &normalizedSize=cv::Size(48,48));
    FaceComponent::Status predictMouthStatusByDNN(cv::Mat mouthChinImg,const cv::Size &normalizedSize = cv::Size(48, 48));
    int predictHeadposeByDNN(cv::Mat faceImg,const cv::Size &normalizedSize=cv::Size(64,64));
    double faceDetectionTime();
	double faceAlignmentTime();
    LBF::BoundingBox convertRectToBoundingBox(const cv::Rect &rect);
private:
        int predictStatusByDNN(cv::Mat image,const cv::Size &normalizedSize, cv::Ptr<CaffeClassifier> &pDnn) {
        cv::resize(image, image, normalizedSize);
        vector<float> probabilities = pDnn->Predict(image);
		int maxIndex = 0;
		for (int i = 0;i < probabilities.size();i++) {
			maxIndex = probabilities[i] > probabilities[maxIndex] ? i : maxIndex;
		}
        return maxIndex;
	}
        cv::Ptr<cv::ml::SVM> loadSVM(const std::string &modelPath);/*{
        #ifdef OPENCV_LT_320
             return cv::ml::SVM::load<cv::ml::SVM>(modelPath);
        #else
            return cv::ml::SVM::load(modelPath);
        #endif
        }*/
};
}
