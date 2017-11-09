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
const string pathFilesFile="imgPathFiles.txt";

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
            //get the image file name
            string imgName=line.substr(line.find_last_of('/')+1,line.length()-line.find_last_of('/'));
          *pFrame=cImg;
            face.resetLastStatus();
            face.detectFaces();
            if(face.bContainFace()){
                face.generateFaceComponentsColorImage();
                cv::imwrite(outputDir+"/"+leftEyeDir+"/L_"+imgName,face.leftEye().colorImg());
                cv::imwrite(outputDir+"/"+rightEyeDir+"/R_"+imgName,face.rightEye().colorImg());
                cv::imwrite(outputDir+"/"+mouthDir+"/M_"+imgName,face.mouth().colorImg());
                cout<<pathPrefix+line<<"processed"<<endl;
            }
        }
}

int main(int argc,char *args[]){
   assert(argc>1);
   configGlobalVariables(args[1]);
    ReadGlobalParamFromFile(projectHome+re_modelHome+featurePointsRegressorModelName);
    ifstream fin(pathPrefix+pathFilesFile);
    assert(fin);
    string pathFileName;
    while(true){
        fin>>pathFileName;
        if(fin.eof()){
            break;
        }
        string dirPrefix=pathFileName.substr(0,pathFileName.find('.'));
        string outputDir=pathPrefix+dirPrefix+"_cut/";
        checkFolder(outputDir);
        cutOutEyesAndMouth(pathPrefix,pathPrefix+pathFileName,outputDir);
    }
    return 0;
}
