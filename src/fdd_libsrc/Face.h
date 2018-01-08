#pragma once
#include "Frame.h"
#include "FaceAnalysisModel.h"
#include "RightEye.h"
#include "LeftEye.h"
#include "Mouth.h"
namespace fdd{
class Face {
public:
    static const cv::Scalar frontColor;
    static const cv::Scalar leftColor;
    static const cv::Scalar rightColor;
private:
    cv::Ptr<Frame> pFrame_;
    cv::Ptr<FaceAnalysisModel> pModel_;
	std::vector<cv::Rect> faces_;
	std::vector<cv::Point> featurePoints_;
	cv::Mat_<double> regressedShapeWithoutScale_;
	RightEye rightEye_;
	LeftEye leftEye_;
	Mouth mouth_;
    bool bContainFrontFace_;
    bool bContainLeftFace_;
    bool bContainRightFace_;
    cv::Rect  faceDetectionRect_;
	cv::Rect maxFaceOnSmallImgForFaceDetection_;
    int headpose_;
public:
	Face();
    Face(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel);
	~Face();
    void pFrame(const cv::Ptr<Frame> &pFrame);
    void pModel(const cv::Ptr<FaceAnalysisModel> &pModel);
    void setFaceDetectionRect(const cv::Rect &faceDetectionRect);
	RightEye &rightEye();
	LeftEye &leftEye();
	Mouth &mouth();
    bool bContainFrontFace();
    bool bContainLeftFace();
    bool bContainRightFace();
    bool bContainFace();
    cv::Rect maxFaceOnSmallImgForFaceDetection();
     int headpose();
     cv::Rect flipRect(const cv::Rect &rect,const cv::Size &imgSize,Frame::Direction direction);
     bool detectFace(FaceAnalysisModel::FaceType faceType);
     void generateFaceComponentsColorImage();
     void analyzeFaceComponentsStatus();
     void drawFaceComponentsRect();
     void analyzeFrontFace();
     void analyzeHeadpose();
     void detectFaces();
     void detectFaces2();
     void drawFaceRect(const cv::Scalar &color);
     void putText(const std::string &text,const cv::Scalar &color);
     void resetLastStatus();
};
}
