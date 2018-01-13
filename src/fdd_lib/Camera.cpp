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
	cv::Mat rawFrame;
    rawFrame=cm_.readFrame();
    fsp_.setFrame(rawFrame);
    //std::cout<<fsp_.getFrameHeight()<<" "<<fsp_.getFrameWidth()<<std::endl;
    fsp_.initProcessor();
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


