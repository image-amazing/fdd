#include"Mouth.h"
#include"FaceAnalysisModel.h"

Mouth::Mouth()
{

}

Mouth::Mouth(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel , double colorImgScale)
	:FaceComponent(pFrame,pModel ,colorImgScale)
{

}

Mouth::~Mouth()
{

}

FaceComponent::Status Mouth::predictStatus()
{
	//return pModel_->predictMouthStatus(colorImg_);
	return pModel_->predictMouthStatusByDNN(colorImg_);
}
