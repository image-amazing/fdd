#include"FrameSequenceProcessor.h"

namespace fdd{

void FrameSequenceProcessor::setFrameWidth(int frameWidth){
    frameWidth_=frameWidth;
}

void FrameSequenceProcessor::setFrameHeight(int frameHeight){
    frameHeight_=frameHeight;
}

int FrameSequenceProcessor::getFrameWidth(){
    return frameWidth_;
}

int FrameSequenceProcessor::getFrameHeight(){
    return frameHeight_;
}

}
