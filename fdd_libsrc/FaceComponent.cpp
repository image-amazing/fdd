#include"FaceComponent.h"
#include"FDDGlobal.h"

FaceComponent::FaceComponent()
	:pFrame_(nullptr),pModel_(nullptr),colorImg_(cv::Mat()),colorImgScale_(1.0f)
{

}

FaceComponent::FaceComponent(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel , double colorImgScale)
	:pFrame_(pFrame),pModel_(pModel),colorImgScale_(colorImgScale)
{

}

FaceComponent::~FaceComponent()
{

}

void FaceComponent::generateColorImg()
{
    //get the non-horizontal bounding rectangle
	cv::RotatedRect rotatedRect = cv::minAreaRect(featurePoints_);
    //extend rectangle
	rotatedRect.size.height *= colorImgScale_;
	rotatedRect.size.width *= colorImgScale_;
	rotatedRect.points(vertices_);
    //rectangle vertexes cordinate
	std::vector<cv::Point2f> rotatedVertices;
	for (int i = 0; i < 4; i++)
	{
		//line(pFrame_->colorImg(), vertices_[i], vertices_[(i + 1) % 4], cv::Scalar(255));
		rotatedVertices.push_back(vertices_[i]);
	}
    //sort rectangle vertexes by cordinate x
	std::sort(rotatedVertices.begin(), rotatedVertices.end(),
		[](const cv::Point2f p0, const cv::Point2f p1)
		-> bool
	{
		return p0.x < p1.x;
	});
    //rotate image by rectangle angle
	cv::Rect rect;
	rect.width = rotatedRect.size.width;
	rect.height = rotatedRect.size.height;
	float rotatedAngle = rotatedRect.angle;
	if (abs(rotatedAngle)>45)
	{
		rotatedAngle += 90;
		rect.width = rotatedRect.size.height;
		rect.height = rotatedRect.size.width;
	}
	cv::Mat rotatedMat = cv::getRotationMatrix2D(rotatedRect.center, rotatedAngle, 1);
	cv::Mat cRotatedImg;
	cv::warpAffine(pFrame_->colorImg(), cRotatedImg, rotatedMat, pFrame_->colorImg().size());
	//cv::imshow("cRotatedImg", cRotatedImg);
	cv::Mat rotatedMatf;
	rotatedMat.convertTo(rotatedMatf, CV_64FC1);
	cv::Point leftTopPoint = rotatedVertices[0].y < rotatedVertices[1].y ? rotatedVertices[0] : rotatedVertices[1];
	rect.x = rotatedMatf.at<double>(0, 0)*leftTopPoint.x + rotatedMatf.at<double>(0, 1)*leftTopPoint.y + rotatedMatf.at<double>(0, 2);
	rect.y = rotatedMatf.at<double>(1, 0)*leftTopPoint.x + rotatedMatf.at<double>(1, 1)*leftTopPoint.y + rotatedMatf.at<double>(1, 2);
    //check boundary to prevent boundary overstepping
    checkBoundary(rect,cv::Rect(0,0,cRotatedImg.size().width,cRotatedImg.size().height));
    /*rect.x = rect.x < 0 ? 0 : rect.x;
	rect.y = rect.y < 0 ? 0 : rect.y;
	rect.width = rect.x + rect.width > cRotatedImg.size().width ? cRotatedImg.size().width - rect.x-1:rect.width;
    rect.height = rect.y + rect.height > cRotatedImg.size().height ? cRotatedImg.size().height - rect.y - 1 : rect.height;*/
    //cut out image
	colorImg_ = cRotatedImg(rect).clone();
}

FaceComponent::Status FaceComponent::analyzeStatus()
{
    //generateColorImg();
	status_ = predictStatus();
	return status_;
} 

cv::Point2f * FaceComponent::vertices() {
	return vertices_;
}

void FaceComponent::drawMinAreaRect() {
	for (int i = 0; i < 4; i++)
	{
		line(pFrame_->colorImg(), vertices_[i], vertices_[(i + 1) % 4], cv::Scalar(255));
	}
}

void FaceComponent::pFrame(const cv::Ptr<Frame> &pFrame)
{
	pFrame_ = pFrame;
}

void FaceComponent::pModel(const cv::Ptr<FaceAnalysisModel> &pModel)
{
	pModel_ = pModel;
}

void FaceComponent::featurePoints(std::vector<cv::Point> &featurePoints)
{
	featurePoints_.clear();
	featurePoints_.assign(featurePoints.begin(),featurePoints.end());
}

void FaceComponent::colorImgScale(double colorImgScale)
{
	colorImgScale_ = colorImgScale;
}

FaceComponent::Status FaceComponent::status()
{
	return status_;
}

cv::Mat FaceComponent::colorImg(){
   return colorImg_;
}
