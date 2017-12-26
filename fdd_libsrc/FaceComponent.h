#pragma once
#include"Frame.h"
#include"FDDGlobal.h"

namespace fdd{
class FaceAnalysisModel;
class FaceComponent {
public:
	 enum Status {open,close};
   const static cv::Scalar normalColor;
   const static cv::Scalar abnormalColor;
protected:
    //raw frame
	Frame *pFrame_;
    //face analysis model
	FaceAnalysisModel *pModel_;
    //feature points for this component
	std::vector<cv::Point> featurePoints_;
    //color image for this component
	cv::Mat colorImg_;
    //component image zoom ratio
	double colorImgScale_;
    //component status
	Status status_;
    //vertexes of  bounding box of the component
	cv::Point2f vertices_[4];
    cv::Point leftTopPoint_;
public:
	FaceComponent();
    FaceComponent(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel,double colorImgScale=1.0);
	virtual ~FaceComponent();
    //generate component image
	void generateColorImg();
    //template method
	FaceComponent::Status analyzeStatus();
    void pFrame(const cv::Ptr<Frame> &pFrame);
    void pModel(const cv::Ptr<FaceAnalysisModel> &pModel);
    void featurePoints(std::vector<cv::Point> &featurePoints);
    void colorImgScale(double colorImgScale);
    FaceComponent::Status status();
    cv::Mat colorImg();
    //get the vertices of the bounding box
	cv::Point2f * vertices();
    //draw the bounding box
    void drawMinAreaRect(const cv::Scalar &color=normalColor);
    void putText(const std::string &text,const cv::Scalar &color=normalColor);
private:
    //predict the status of the component
    virtual FaceComponent::Status predictStatus() = 0;
};
}
