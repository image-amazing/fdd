#pragma once
#include"FaceComponent.h"
namespace fdd{
class FaceAnalysisModel;
class RightEye:public FaceComponent{
public:
	RightEye();
    RightEye(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel , double colorImgScale = 1.0);
	~RightEye() override;
private:
	FaceComponent::Status predictStatus() override;
};
}
