#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<dm_global.h>
#include<Frame.h>
#include<FaceAnalysisModel.h>
#include<Face.h>
#include<assert.h>
#include<time.h>
#include<stdlib.h>
using namespace std;
using namespace dm;
using namespace fdd;
using namespace LBF;

const string leftEyeDir="LeftEye/";
const string rightEyeDir="RightEye/";
const string mouthDir="Mouth/";

  DriverMonitorConfigure dmc;

void cutOutEyesAndMouth(const string &pathPrefix,const string &pathFile,const string &outputDir){
       checkFolder(outputDir+"/"+leftEyeDir);
       checkFolder(outputDir+"/"+rightEyeDir);
       checkFolder(outputDir+"/"+mouthDir);
       cv::Ptr<Frame> pFrame(new Frame());
       pFrame->setScaleForFaceDetection(0.2);
       cv::Ptr<FaceAnalysisModel> pfam(new FaceAnalysisModel());
        string ab_modelPath=dmc.projectHome+dmc.re_modelHome;
        pfam->loadFCC(ab_modelPath+dmc.fccModelName);
        pfam->loadPFCC(ab_modelPath+dmc.pfccModelName);
        pfam->loadFeaturePointsRegressor(ab_modelPath+dmc.featurePointsRegressorModelName,ab_modelPath+dmc.regName);

        Face face(pFrame,pfam);
        face.leftEye().colorImgScale(1.3);
        face.rightEye().colorImgScale(1.3);
        ifstream fin(pathFile);
        assert(fin);
        string line;
        unsigned int imgCount=0;
        while(getline(fin,line)){
            cv::Mat cImg=cv::imread(pathPrefix+line);
            if(cImg.data==NULL){
                cout<<pathPrefix+line<<" skipped"<<endl;
                    continue;
            }
            //get the image file name
            string imgName=line.substr(line.find_last_of('/')+1,line.length()-line.find_last_of('/'));
          *pFrame=cImg;
            face.resetLastStatus();
            face.detectFaces();
            if(face.bContainFrontFace()){
                //cout<<face.bContainFrontFace()<<","<<face.bContainLeftFace()<<","<<face.bContainRightFace()<<endl;
                face.generateFaceComponentsColorImage();
                face.drawFaceComponentsRect();
                face.drawFaceRect(Face::frontColor);
                time_t nowTime=time(0);
                 char strLabel[32];
                 sprintf(strLabel,"%08u_%u_",imgCount,static_cast<unsigned int>(nowTime));
                cv::imwrite(outputDir+"/"+leftEyeDir+"/L_"+strLabel+imgName,face.leftEye().colorImg());
                cv::imwrite(outputDir+"/"+rightEyeDir+"/R_"+strLabel+imgName,face.rightEye().colorImg());
                cv::imwrite(outputDir+"/"+mouthDir+"/M_"+strLabel+imgName,face.mouth().colorImg());
                cv::imshow("colorImg",pFrame->colorImg());
                cv::waitKey(1);
                cout<<pathPrefix+line<<"  processed"<<endl;
            }
            imgCount++;
        }
       fin.close();
}

int main(int argc,char *args[]){
   assert(4==argc);
   string configFile=args[1];
   string pathPrefix=args[2];
   string pathFilesFile=args[3];
   dmc=configGlobalVariables(configFile);
   ReadGlobalParamFromFile(dmc.projectHome+dmc.re_modelHome+dmc.featurePointsRegressorModelName);
    ifstream fin(pathFilesFile);
    assert(fin);
    string pathFileName;
    while(getline(fin,pathFileName)){
        string dirPrefix=pathFileName.substr(0,pathFileName.find('.'));
        string outputDir=pathPrefix+dirPrefix+"_cut/";
        checkFolder(outputDir);
        cutOutEyesAndMouth(pathPrefix,pathPrefix+pathFileName,outputDir);
    }
    fin.close();
    return 0;
}
