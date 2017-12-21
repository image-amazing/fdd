#include <iostream>
#include<fstream>
#include<assert.h>
#include<opencv2/opencv.hpp>
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
    //ofstream fout((pathPrefix+"/G_"+pathFile).c_str());
    assert(fin);
    string line;
    unsigned int imgCount=0;
    while(getline(fin,line)){
        cv::Mat cImg=cv::imread(pathPrefix+"/"+line);
        if(NULL==cImg.data){
            continue;
        }
        cv::Mat gImg;
        cv::cvtColor(cImg,gImg,CV_BGR2GRAY);
        cv::equalizeHist(gImg,gImg);
        //string filename=line.substr(line.find_last_of('/')+1,line.length()-line.find_last_of('/'));
        time_t nowTime=time(0);
        char filename[64];
        sprintf(filename,"%08u_%u.jpg",imgCount , static_cast<unsigned int>(nowTime));
        string relativePath=outputDir+"/G_"+filename;
        cv::imwrite(pathPrefix+"/"+relativePath,gImg);
        cout<<pathPrefix+"/"+relativePath<<" writed"<<endl;
        imgCount++;
    }
    fin.close();
    return 0;
}
