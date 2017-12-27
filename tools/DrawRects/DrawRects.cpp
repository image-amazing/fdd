#include <iostream>
#include<opencv2/opencv.hpp>
#include<dm_global.h>
#include<Frame.h>
#include<FaceAnalysisModel.h>
#include<Face.h>
#include<assert.h>

using namespace std;
using namespace dm;
using namespace fdd;
using namespace LBF;

int main(int argc,char *args[])
{
    assert(4==argc);
    DriverMonitorConfigure dmc = configGlobalVariables(args[1]);
    string inputFile=args[2];
    string outputFile=args[3];
    string ab_modelPath=dmc.projectHome+dmc.re_modelHome;
    //initialize lbf global parameter
    ReadGlobalParamFromFile(ab_modelPath+dmc.featurePointsRegressorModelName);


    cv::Ptr<Frame> pFrame(new Frame());
    pFrame->setScaleForFaceDetection(0.2);
    cv::Ptr<FaceAnalysisModel> pfam(new FaceAnalysisModel());
    pfam->loadFCC(ab_modelPath+dmc.fccModelName);
    pfam->loadPFCC(ab_modelPath+dmc.pfccModelName);
    pfam->loadFeaturePointsRegressor(ab_modelPath+dmc.featurePointsRegressorModelName,ab_modelPath+dmc.regName);

    Face face(pFrame,pfam);
    face.leftEye().colorImgScale(1.3);
    face.rightEye().colorImgScale(1.3);
    cv::Mat inputImg=cv::imread(inputFile.c_str());
    assert(NULL!=inputImg.data);
    *pFrame=inputImg;
    face.detectFaces2();
    if(face.bContainFrontFace()){
        face.generateFaceComponentsColorImage();
        face.drawFaceComponentsRect();
        face.drawFaceRect(Face::frontColor);
        cv::imshow(outputFile,pFrame->colorImg());
        cv::waitKey(0);
        cv::imwrite(outputFile,pFrame->colorImg());
        cout<<outputFile<<" writed"<<endl;
    }else{
        cout<<"no face detected!"<<endl;
    }

    return 0;
}






