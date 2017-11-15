#include"Camera.h"
namespace fdd{
Camera::Camera(FrameSequenceProcessor &fps,int cameraID)
    :fsp_(fps),cameraID_(cameraID)
{
	
}

Camera::~Camera()
{

}

void Camera::run()
{
    cm_.openCamera(cameraID_);
    fsp_.beforeProcess();
	cv::Mat rawFrame;
	while (true)
	{
		rawFrame = cm_.readFrame();
		fsp_.process(rawFrame);
	}
}

void Camera::setCameraID(int cameraID){
    cameraID_=cameraID;
}

int Camera::getCameraID(){
    return cameraID_;
}
}


