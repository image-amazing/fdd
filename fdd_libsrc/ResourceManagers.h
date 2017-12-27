#pragma once
#include<opencv2/opencv.hpp>
namespace fdd{
//manage camera resource
class CameraManager {
public:
	CameraManager();
	~CameraManager();
	cv::Mat &readFrame();
	int getFrameWidth();
	int getFrameHeight();
    void openCamera(int cameraID=0);
	cv::Mat frame_;
private:
	cv::VideoCapture capture;
};

//manage video resource
class VideoManager {
public:
	VideoManager();
    VideoManager(const std::string & videoPath0,int fourcc0,int fps0,
		cv::Size frameSize0,bool bAdd0);
	~VideoManager();
	void setVideoPath(const std::string &videoPath0);
	void setFourcc(int fourcc0);
	void setFPS(int fps0);
	void setFrameSize(const cv::Size &frameSize0);
	void setBAdd(bool bAdd);
	const std::string &getVideoPath() const;
    const std::string getVideoName() const;
	void open();
	void release();
    void write(const cv::Mat &frame);
private:
	cv::VideoWriter writer_;
	std::string videoPath_;
	int fourcc_;
	int fps_;
	cv::Size frameSize_;
	bool bAdd_;
};
}
