#pragma once
#include"FaceComponent.h"
class FaceAnalysisModel;
class LeftEye :public FaceComponent {
public:
	LeftEye();
    LeftEye(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel, double colorImgScale = 1.0);
	~LeftEye() override;
private:
	FaceComponent::Status predictStatus() override;
};
