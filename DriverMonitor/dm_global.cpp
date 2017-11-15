#include"dm_global.h"
#include<iostream>
#include<libconfig.h++>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdio.h>
#include<assert.h>
#include<cstring>

namespace dm{

  std::string projectHome;
//model root folder
  std::string re_modelHome;
//front face detection model
  std::string fccModelName;
//profile face detection model
  std::string pfccModelName;
  std::string rightEyeStatusSVMModelName;
  std::string mouthChinStatusSVMModelName;
  std::string featurePointsRegressorModelName;
  std::string regName;
  std::string mediaHome;
  std::string rightEyeFatigueAudio;
  std::string mouthFatigueAudio;
  std::string outputDataHome;
  std::string videoFolder;
  std::string eyesEvidenceFolder;
  std::string mouthEvidenceFolder;
  std::string rightEyeDNNModelFile;
  std::string rightEyeDNNWeightsFile;
  std::string rightEyeDNNMeanFile;
  std::string mouthChinDNNModelFile;
  std::string mouthChinDNNWeightsFile;
  std::string mouthChinDNNMeanFile;
  std::string leftEyeDNNModelFile;
  std::string leftEyeDNNWeightsFile;
  std::string leftEyeDNNMeanFile;
  std::string faceComponentLabelFile;
  std::string headposeDNNModelFile;
  std::string headposeDNNWeightsFile;
  std::string headposeDNNMeanFile;
  std::string headposeLabelFile;

//config global parameters
void configGlobalVariables(const std::string &configFile) {
    try {
        libconfig::Config cfg;
        cfg.readFile(configFile.c_str());
        projectHome=cfg.lookup("projectHome").c_str();
        re_modelHome = cfg.lookup("re_modelHome").c_str();
        fccModelName = cfg.lookup("fccModelName").c_str();
        pfccModelName=cfg.lookup("pfccModelName").c_str();
        rightEyeStatusSVMModelName = cfg.lookup("rightEyeStatusSVMModelName").c_str();
        mouthChinStatusSVMModelName = cfg.lookup("mouthChinStatusSVMModelName").c_str();
        featurePointsRegressorModelName = cfg.lookup("featurePointsRegressorModelName").c_str();
        regName=cfg.lookup("regName").c_str();
        mediaHome = cfg.lookup("mediaHome").c_str();
        rightEyeFatigueAudio = cfg.lookup("rightEyeFatigueAudio").c_str();
        mouthFatigueAudio = cfg.lookup("mouthFatigueAudio").c_str();
        outputDataHome=cfg.lookup("outputDataHome").c_str();
        videoFolder = cfg.lookup("videoFolder").c_str();
        eyesEvidenceFolder=cfg.lookup("eyesEvidenceFolder").c_str();
        mouthEvidenceFolder=cfg.lookup("mouthEvidenceFolder").c_str();
        rightEyeDNNModelFile = cfg.lookup("rightEyeDNNModelFile").c_str();
        rightEyeDNNWeightsFile = cfg.lookup("rightEyeDNNWeightsFile").c_str();
        rightEyeDNNMeanFile = cfg.lookup("rightEyeDNNMeanFile").c_str();
        mouthChinDNNModelFile = cfg.lookup("mouthChinDNNModelFile").c_str();
        mouthChinDNNWeightsFile = cfg.lookup("mouthChinDNNWeightsFile").c_str();
        mouthChinDNNMeanFile = cfg.lookup("mouthChinDNNMeanFile").c_str();
        leftEyeDNNModelFile = cfg.lookup("leftEyeDNNModelFile").c_str();
        leftEyeDNNWeightsFile = cfg.lookup("leftEyeDNNWeightsFile").c_str();
        leftEyeDNNMeanFile = cfg.lookup("leftEyeDNNMeanFile").c_str();
        faceComponentLabelFile = cfg.lookup("faceComponentLabelFile").c_str();
        headposeDNNModelFile=cfg.lookup("headposeDNNModelFile").c_str();
        headposeDNNWeightsFile=cfg.lookup("headposeDNNWeightsFile").c_str();
        headposeDNNMeanFile=cfg.lookup("headposeDNNMeanFile").c_str();
        headposeLabelFile="headposeLabels.txt";
    }
    catch (...) {
        std::cout << "fail to config from "<<configFile.c_str() <<std:: endl;
    }
}

int removeContents(const std::string &path){
    DIR *dir=opendir(path.c_str());
    assert(NULL!=dir);
    struct dirent *ptr=readdir(dir);
    while(NULL!=ptr){
        if(0!=strcmp(ptr->d_name,".")&&0!=strcmp(ptr->d_name,"..")){
            if(4==ptr->d_type){
                if(-1==removeContents((path+"/"+ptr->d_name).c_str())|| -1==rmdir((path+"/"+ptr->d_name).c_str())){
                      return -1;
                }
            }else{
                if(-1==remove((path+"/"+ptr->d_name).c_str())){
                    return -1;
                }
            }
        }
        ptr=readdir(dir);
    }
    return 0;
}

//check if folder exists,if not ,make the folder
void checkFolder(const std::string &folderName,bool bRetain){
    //determine if the folder exists
    if(-1==access(folderName.c_str(),F_OK)){
        //folder doesn't exists
        if(-1==mkdir(folderName.c_str(),S_IRWXU)){
            //fail to mkdir folder
                std::cout<<"fail to mkdir "<<folderName.c_str()<<std::endl;
                exit(-1);
        }
    }else{
        if(!bRetain){
            removeContents(folderName);
        }
    }
}


}
