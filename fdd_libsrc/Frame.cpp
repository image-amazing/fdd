#include"Frame.h"
namespace fdd{
Frame::Frame()
	:rawFrame_(cv::Mat()),colorImg_(cv::Mat()),grayImg_(cv::Mat())
	,graySmallImgForFaceDetection_(cv::Mat()),scaleForFaceDetection_(0.2)
{

}

Frame::Frame(const cv::Mat &rawFrame,double scaleForFaceDetection)
	:rawFrame_(rawFrame),scaleForFaceDetection_(scaleForFaceDetection)
{
	fillImgs();
}
//generate images according to raw frame
inline void Frame::fillImgs()
{
	colorImg_ = rawFrame_.clone();
	cv::cvtColor(rawFrame_, grayImg_, CV_BGR2GRAY);
	cv::resize(grayImg_, graySmallImgForFaceDetection_, cv::Size(grayImg_.size().width*scaleForFaceDetection_, grayImg_.size().height*scaleForFaceDetection_));
}

Frame::~Frame()
{

}

Frame &Frame::operator=(const cv::Mat &rawFrame)
{
	rawFrame_ = rawFrame;
	fillImgs();
	return *this;
}

void Frame::setScaleForFaceDetection(double scaleForFaceDetection)
{
	scaleForFaceDetection_ = scaleForFaceDetection;
}

double Frame::getScaleForFaceDetection()
{
	return scaleForFaceDetection_;
}

cv::Mat &Frame::rawFrame()
{
	return rawFrame_;
}

cv::Mat &Frame::colorImg()
{
	return colorImg_;
}

cv::Mat &Frame::grayImg()
{
	return grayImg_;
}

cv::Mat &Frame::graySmallImgForFaceDetection()
{
	return graySmallImgForFaceDetection_;
}

void Frame::equalizeGraySmallImgForFaceDetectionHist()
{
	cv::equalizeHist(graySmallImgForFaceDetection_, graySmallImgForFaceDetection_);
}
}
