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
	//return pModel_->predictRightEyeStatus(colorImg_);
	return pModel_->predictLeftEyeStatusByDNN(colorImg_);
}
}
