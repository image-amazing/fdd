#pragma once
#include"FaceComponent.h"
namespace fdd{
class FaceAnalysisModel;
class Mouth: public FaceComponent{
public:
	Mouth();
    Mouth(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel , double colorImgScale = 1.0);
	~Mouth() override;
private:
	FaceComponent::Status predictStatus() override;
};
}
