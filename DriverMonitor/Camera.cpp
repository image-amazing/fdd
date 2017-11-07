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
    fsp_.beforeProcess();
	cv::Mat rawFrame;
	while (true)
	{
		rawFrame = cm_.readFrame();
		fsp_.process(rawFrame);
	}
}
