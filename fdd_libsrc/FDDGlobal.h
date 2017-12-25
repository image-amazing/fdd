#ifndef FDDGLOBAL_H
#define FDDGLOBAL_H
#include"opencv2/opencv.hpp"
namespace fdd{
void checkBoundary(cv::Rect &checkedRect , const cv::Rect &boundingRect);
cv::Rect extendRect(const cv::Rect &originRect,double scale);
const cv::Scalar normalColor(0,255,0);
const cv::Scalar abnormalColor(0,0,255);
}
#endif
