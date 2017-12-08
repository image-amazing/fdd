#include <iostream>
#include<fstream>
#include<assert.h>
#include<opencv2/opencv.hpp>
#include<dm_global.h>

using namespace std;

int main(int argc,char *args[])
{
    assert(4==argc);
    string root=args[1];
    string pathFile=args[2];
    string outputDir=args[3];
    dm::checkFolder(root+"/"+outputDir);
    ifstream fin((root+"/"+pathFile).c_str());
    ofstream fout((root+"/G_"+pathFile).c_str());
    assert(fin);
    string line;
    int label;
    while(fin>>line&&fin>>label){
        cv::Mat cImg=cv::imread(root+"/"+line);
        if(NULL==cImg.data){
            continue;
        }
        cv::Mat gImg;
        cv::cvtColor(cImg,gImg,CV_BGR2GRAY);
        cv::equalizeHist(gImg,gImg);
        string filename=line.substr(line.find_last_of('/')+1,line.length()-line.find_last_of('/'));
        string relativePath;
        if(0==label){
            relativePath=outputDir+"/G_O_"+filename;
        }else{
            relativePath=outputDir+"/G_C_"+filename;
        }
        cv::imwrite(root+"/"+relativePath,gImg);
        cout<<root+"/"+relativePath<<" writed"<<endl;
        fout<<relativePath<<' '<<label<<endl;
    }
    fin.close();
    return 0;
}

