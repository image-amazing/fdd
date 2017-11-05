#include"Camera.h"

Camera::Camera(FrameSequenceProcessor &fps)
    :fsp_(fps)
{
	
}

Camera::~Camera()
{

}

void Camera::run()
{
	cm_.openCamera();
    fsp_.getVm().setFourcc(CV_FOURCC('M','P','4','2'));
    fsp_.getVm().setFrameSize(cv::Size(cm_.getFrameWidth(),cm_.getFrameHeight()));
    fsp_.getVm().setBAdd(true);
    fsp_.getEevm().setFourcc(CV_FOURCC('M','P','4','2'));
    fsp_.getEevm().setFrameSize(cv::Size(cm_.getFrameWidth(),cm_.getFrameHeight()));
    fsp_.getEevm().setBAdd(true);
    fsp_.getMevm().setFourcc(CV_FOURCC('M','P','4','2'));
    fsp_.getMevm().setFrameSize(cv::Size(cm_.getFrameWidth(),cm_.getFrameHeight()));
    fsp_.getMevm().setBAdd(true);
	cv::Mat rawFrame;
	while (true)
	{
		rawFrame = cm_.readFrame();
		fsp_.process(rawFrame);
	}
}
