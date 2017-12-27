#include "ResourceManagers.h"
#include<exception>

namespace fdd{

CameraManager::CameraManager()
{
	
}

void CameraManager::openCamera(int cameraID)
{
    capture.open(cameraID);
}

cv::Mat &CameraManager::readFrame()
{
	if (this->capture.isOpened())
	{
		
		this->capture >> frame_;
		return frame_;
	}
	else
	{
		throw std::exception();
	}
}

CameraManager::~CameraManager()
{
	try {
		if (this->capture.isOpened())
		{
			this->capture.release();
		}
	}
	catch (...)
	{

	}
}

int CameraManager::getFrameWidth()
{
	return this->capture.get(CV_CAP_PROP_FRAME_WIDTH);
}

int CameraManager::getFrameHeight()
{
	return this->capture.get(CV_CAP_PROP_FRAME_HEIGHT);
}

VideoManager::VideoManager()
	:videoPath_(""),fourcc_(0),fps_(0),frameSize_(cv::Size()),bAdd_(true)
{

}

VideoManager::VideoManager(const std::string & videoPath0, int fourcc0, int fps0, cv::Size frameSize0,bool bAdd0)
	:videoPath_(videoPath0), fourcc_(fourcc0), fps_(fps0), frameSize_(frameSize0),bAdd_(bAdd0)
{

}

VideoManager::~VideoManager()
{
	try {
		release();
	}
	catch (...)
	{

	}
	
}

void VideoManager::setVideoPath(const std::string &videoPath0)
{
	videoPath_ = videoPath0;
}

void VideoManager::setFourcc(int fourcc0)
{
	fourcc_ = fourcc0;
}

void VideoManager::setFPS(int fps0)
{
	fps_ = fps0;
}

void VideoManager::setFrameSize(const cv::Size &frameSize0)
{
	frameSize_ = frameSize0;
}

void VideoManager::setBAdd(bool bAdd)
{
	bAdd_ = bAdd;
}

void VideoManager::open()
{
	writer_.open(videoPath_, fourcc_, fps_, frameSize_,bAdd_);
}

void VideoManager::release()
{
	if (writer_.isOpened())
	{
		writer_.release();
	}
}

void VideoManager::write(const cv::Mat &frame)
{
	writer_ << frame;
}

const std::string &VideoManager::getVideoPath() const
{
	return videoPath_;
}

const std::string VideoManager::getVideoName() const{
    return videoPath_.substr(videoPath_.find_last_of('/')+1,videoPath_.length()-videoPath_.find_last_of('/')-1);
}
}
