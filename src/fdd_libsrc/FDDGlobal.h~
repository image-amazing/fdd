#ifndef FDDGLOBAL_H
#define FDDGLOBAL_H
#include"opencv2/opencv.hpp"
#include<time.h>
namespace fdd{
void checkBoundary(cv::Rect &checkedRect , const cv::Rect &boundingRect);
cv::Rect extendRect(const cv::Rect &originRect,double scale);

inline std::string getTimeStr(time_t time_seconds){
    struct tm *now_time=localtime(&time_seconds);
     std::ostringstream sout;
     sout<<now_time->tm_year+1900<<"-"<<now_time->tm_mon<<"-"<<now_time->tm_mday
        <<"_"<<now_time->tm_hour<<"."<<now_time->tm_min<<"."<<now_time->tm_sec;
     std::string strTime=sout.str();
     return strTime;
}

inline std::string getNowTimeStr(){
    time_t time_seconds=time(0);
    return getTimeStr(time_seconds);
}

const int PATH_LENGTH=256;

struct FatigueMessage{
    char resultFileName_[PATH_LENGTH];
    char evidenceName_[PATH_LENGTH];
    void setResultFileName(const std::string &resultFileName){
        strncpy(resultFileName_,resultFileName.c_str(),resultFileName.length());
        resultFileName_[resultFileName.length()]='\0';
    }
    void setEvidenceName(const std::string &evidenceName){
        strncpy(evidenceName_,evidenceName.c_str(),evidenceName.length());
        evidenceName_[evidenceName.length()]='\0';
    }
    const std::string getResultFileName() const{
           return resultFileName_;
    }
    const std::string getEvidenceName() const{
           return evidenceName_;
    }
};

}
#endif
