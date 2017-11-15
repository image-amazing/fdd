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
	//return pModel_->predictRightEyeStatus(colorImg_);
	return pModel_->predictRightEyeStatusByDNN(colorImg_);
}
}
