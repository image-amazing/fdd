#include"FDDGlobal.h"
namespace fdd{

void checkBoundary(cv::Rect &checkedRect , const cv::Rect &boundingRect){
    //check checkedRect with boundingRect to prevent overstep the boundary
    checkedRect.x = checkedRect.x < boundingRect.x? boundingRect.x : checkedRect.x;
    checkedRect.y = checkedRect.y < boundingRect.y? boundingRect.y : checkedRect.y;
    checkedRect.width = checkedRect.x + checkedRect.width > boundingRect.width ? boundingRect.width - checkedRect.x-1:checkedRect.width;
    checkedRect.height = checkedRect.y + checkedRect.height > boundingRect.height ? boundingRect.height - checkedRect.y - 1 : checkedRect.height;
}

cv::Rect extendRect(const cv::Rect &originRect,double scale){
//extend originRect by scale
   cv::Rect extendedRect;
   extendedRect.x=originRect.x-originRect.width*(scale-1)/2.0f;
   extendedRect.y=originRect.y-originRect.height*(scale-1)/2.0f;
   extendedRect.width=originRect.width*scale;
   extendedRect.height=originRect.height*scale;
   return extendedRect;
}
}
