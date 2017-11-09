#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<dm_global.h>
#include<Frame.h>
#include<FaceAnalysisModel.h>
#include<Face.h>
#include<assert.h>
using namespace std;
const string leftEyeDir="LeftEye/";
const string rightEyeDir="RightEye/";
const string mouthDir="Mouth/";
const string pathPrefix="/home/lyfe667/datasets/";
const string pathFileName="imgPaths.txt";

void cutOutEyesAndMouth(const string &pathPrefix,const string &pathFile,const string &outputDir){
       checkFolder(outputDir+"/"+leftEyeDir);
       checkFolder(outputDir+"/"+rightEyeDir);
       checkFolder(outputDir+"/"+mouthDir);
       cv::Ptr<Frame> pFrame(new Frame());
       pFrame->setScaleForFaceDetection(0.2);
       cv::Ptr<FaceAnalysisModel> pfam(new FaceAnalysisModel());
        string ab_modelPath=projectHome+re_modelHome;
        pfam->loadFCC(ab_modelPath+fccModelName);
        pfam->loadPFCC(ab_modelPath+pfccModelName);
        pfam->loadFeaturePointsRegressor(ab_modelPath+featurePointsRegressorModelName,ab_modelPath+regName);

        Face face(pFrame,pfam);
        ifstream fin(pathFile);
        assert(fin);
        string line;
        while(true){
            fin>>line;
            if(fin.eof()){
                break;
            }
            cv::Mat cImg=cv::imread(pathPrefix+line);
          *pFrame=cImg;
            face.resetLastStatus();
            face.detectFaces();
            if(face.bContainFace()){
                 cout<<"+++"<<endl;
                face.analyzeFrontFace();
                cout<<"---"<<endl;
                cv::imshow("leftEye",face.leftEye().colorImg());
                cv::imshow("rightEye",face.rightEye().colorImg());
                cv::imshow("mouth",face.mouth().colorImg());
                cv::imshow("colorImg",pFrame->colorImg());
                cv::waitKey();
            }
        }
}

int main(int argc,char *args[]){
   assert(argc>1);
   configGlobalVariables(args[1]);
    ReadGlobalParamFromFile(projectHome+re_modelHome+featurePointsRegressorModelName);
    cutOutEyesAndMouth(pathPrefix,pathPrefix+pathFileName,pathPrefix);
    return 0;
}
