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

//config global parameters
DriverMonitorConfigure configGlobalVariables(const std::string &configFile) {
    try {
        DriverMonitorConfigure dmc;
        libconfig::Config cfg;
        cfg.readFile(configFile.c_str());
        dmc.projectHome=cfg.lookup("projectHome").c_str();
         dmc.re_modelHome = cfg.lookup("re_modelHome").c_str();
         dmc.fccModelName = cfg.lookup("fccModelName").c_str();
         dmc.pfccModelName=cfg.lookup("pfccModelName").c_str();
         dmc.rightEyeStatusSVMModelName = cfg.lookup("rightEyeStatusSVMModelName").c_str();
        dmc.mouthChinStatusSVMModelName = cfg.lookup("mouthChinStatusSVMModelName").c_str();
         dmc.featurePointsRegressorModelName = cfg.lookup("featurePointsRegressorModelName").c_str();
         dmc.regName=cfg.lookup("regName").c_str();
         dmc.outputDataHome=cfg.lookup("outputDataHome").c_str();
         dmc.videoFolder = cfg.lookup("videoFolder").c_str();
          dmc.eyeEvidenceFolder=cfg.lookup("eyeEvidenceFolder").c_str();
         dmc.mouthEvidenceFolder=cfg.lookup("mouthEvidenceFolder").c_str();
         dmc.resultFolder=cfg.lookup("resultFolder").c_str();
         dmc.rightEyeDNNModelFile = cfg.lookup("rightEyeDNNModelFile").c_str();
         dmc.rightEyeDNNWeightsFile = cfg.lookup("rightEyeDNNWeightsFile").c_str();
         dmc.rightEyeDNNMeanFile = cfg.lookup("rightEyeDNNMeanFile").c_str();
         dmc.mouthChinDNNModelFile = cfg.lookup("mouthChinDNNModelFile").c_str();
         dmc.mouthChinDNNWeightsFile = cfg.lookup("mouthChinDNNWeightsFile").c_str();
         dmc.mouthChinDNNMeanFile = cfg.lookup("mouthChinDNNMeanFile").c_str();
         dmc.leftEyeDNNModelFile = cfg.lookup("leftEyeDNNModelFile").c_str();
         dmc.leftEyeDNNWeightsFile = cfg.lookup("leftEyeDNNWeightsFile").c_str();
        dmc.leftEyeDNNMeanFile = cfg.lookup("leftEyeDNNMeanFile").c_str();
         dmc.faceComponentLabelFile = cfg.lookup("faceComponentLabelFile").c_str();
         dmc.headposeDNNModelFile=cfg.lookup("headposeDNNModelFile").c_str();
         dmc.headposeDNNWeightsFile=cfg.lookup("headposeDNNWeightsFile").c_str();
         dmc.headposeDNNMeanFile=cfg.lookup("headposeDNNMeanFile").c_str();
         dmc.headposeLabelFile=cfg.lookup("headposeLabelFile").c_str();
         dmc.cameraID=cfg.lookup("cameraID").c_str();
         dmc.resultMessageQueueKey=cfg.lookup("resultMessageQueueKey").c_str();
         dmc.resultMessageProcessorPath=cfg.lookup("resultMessageProcessorPath").c_str();
         return dmc;
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
