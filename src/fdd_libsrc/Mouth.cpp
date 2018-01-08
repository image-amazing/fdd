#include"Mouth.h"
#include"FaceAnalysisModel.h"

namespace fdd{

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
    cv::Mat grayImg;
    cv::cvtColor(colorImg_,grayImg,CV_BGR2GRAY);
    cv::equalizeHist(grayImg,grayImg);
    return pModel_->predictMouthStatusByDNN(grayImg);
}
}
