#ifndef FRAMESEQUENCEPROCESSOR_H
#define FRAMESEQUENCEPROCESSOR_H
#include<opencv2/opencv.hpp>
#include "Frame.h"

namespace fdd{

class FrameSequenceProcessor{
protected:
    Frame frame_;
    int frameWidth_;
    int frameHeight_;
public:
    virtual void initProcessor()=0;
    virtual void beforeProcess()=0;
    virtual void process(cv::Mat rawFrame)=0;
    virtual void afterProcess()=0;
public:
    void setFrameWidth(int frameWidth);
    void setFrameHeight(int frameHeight);
    int getFrameWidth();
    int getFrameHeight();
    void setFrame(cv::Mat &rawFrame);
};
}
#endif // FRAMESEQUENCEPROCESSOR_H
