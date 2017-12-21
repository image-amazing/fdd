#include <iostream>
#include<opencv2/opencv.hpp>
#include<fstream>
#include<dm_global.h>
#include<time.h>
using namespace std;

int main(int argc,char *args[])
{
    assert(4==argc);
    string pathPrefix=args[1];
    string pathFile=args[2];
    string outputDir=args[3];
    dm::checkFolder(pathPrefix+"/"+outputDir);
    ifstream fin(pathFile.c_str());
    assert(fin);
    string line;
    unsigned int imgCount=0;
    while(getline(fin,line)){
        cv::Mat cImg=cv::imread(pathPrefix+"/"+line);
        if(NULL==cImg.data){
            cout<<pathPrefix+"/"+line<<" skiped";
            continue;
        }
        vector<cv::Mat> imgs;
        cv::split(cImg,imgs);
        cv::equalizeHist(imgs[0],imgs[0]);
        cv::equalizeHist(imgs[1],imgs[1]);
        cv::equalizeHist(imgs[2],imgs[2]);
        //string filename=line.substr(line.find_last_of('/')+1,line.length()-line.find_last_of('/'));
        time_t nowTime=time(0);
        char filename[64];
        sprintf(filename,"%08u_%u.jpg",imgCount,static_cast<unsigned int>(nowTime));
        cv::imwrite(pathPrefix+"/"+outputDir+"/channel0_"+filename,imgs[0]);
        cout<<pathPrefix+"/"+outputDir+"/channel0_"+filename<<" writed"<<endl;
        cv::imwrite(pathPrefix+"/"+outputDir+"/channel1_"+filename,imgs[1]);
        cout<<pathPrefix+"/"+outputDir+"/channel1_"+filename<<" writed"<<endl;
        cv::imwrite(pathPrefix+"/"+outputDir+"/channel2_"+filename,imgs[2]);
        cout<<pathPrefix+"/"+outputDir+"/channel2_"+filename<<" writed"<<endl;
        imgCount++;
    }
    fin.close();
    return 0;
}

