#pragma once
#include"ResourceManagers.h"
#include"FrameSequenceProcessor.h"
namespace fdd{
class Camera {
private:
	CameraManager cm_;
    int cameraID_;
    FrameSequenceProcessor &fsp_;
public:
    Camera(FrameSequenceProcessor &fps ,int cameraID=0);
	~Camera();
	void run();
    void setCameraID(int cameraID);
    int getCameraID();
};
}
