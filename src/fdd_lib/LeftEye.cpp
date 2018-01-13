#include"LeftEye.h"
#include "FaceAnalysisModel.h"
namespace fdd{
LeftEye::LeftEye()
{

}

LeftEye::LeftEye(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel, double colorImgScale)
	:FaceComponent(pFrame, pModel, colorImgScale)
{

}

LeftEye::~LeftEye()
{

}

FaceComponent::Status LeftEye::predictStatus()
{
    cv::Mat grayImg;
    cv::cvtColor(colorImg_,grayImg,CV_BGR2GRAY);
    cv::equalizeHist(grayImg,grayImg);
    return pModel_->predictLeftEyeStatusByDNN(grayImg);
}
}
