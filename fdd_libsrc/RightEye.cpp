#include"RightEye.h"
#include "FaceAnalysisModel.h"
namespace fdd{
RightEye::RightEye()
{

}

RightEye::RightEye(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel , double colorImgScale)
	:FaceComponent(pFrame, pModel , colorImgScale)
{

}

RightEye::~RightEye()
{
	
}

FaceComponent::Status RightEye::predictStatus()
{
    cv::Mat grayImg;
    cv::cvtColor(colorImg_,grayImg,CV_BGR2GRAY);
    cv::equalizeHist(grayImg,grayImg);
    return pModel_->predictRightEyeStatusByDNN(grayImg);
}
}
