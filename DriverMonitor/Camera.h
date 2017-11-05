#pragma once
#include"ResourceManagers.h"
#include"FrameSequenceProcessor.h"
class Camera {
private:
	CameraManager cm_;
    FrameSequenceProcessor &fsp_;
public:
    Camera(FrameSequenceProcessor &fps );
	~Camera();
	void run();
};
