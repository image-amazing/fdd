#include<iostream>
#include<fstream>
#include"LBFRegressor.h"
#include"Camera.h"
#include"variables.h"
#include<libconfig.h++>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>


//config global parameters
void configGlobalVariables(const string &configFile) {
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
//check if folder exists,if not ,make the folder
inline void checkFolder(const string &folderName){
    //determine if the folder exists
    if(-1==access(folderName.c_str(),F_OK)){
        //folder doesn't exists
        if(-1==mkdir(folderName.c_str(),S_IRWXU)){
            //fail to mkdir folder
                std::cout<<"fail to mkdir "<<folderName.c_str()<<std::endl;
                exit(-1);
        }
    }
}

int main(int argc,char *args[])
{
      if (argc<=1) {
       std::cout << "config file needed!" << std::endl;
	return -1;
      }
	configGlobalVariables(args[1]); 
    checkFolder(outputDataHome+videoFolder);
    checkFolder(outputDataHome+eyesEvidenceFolder);
    checkFolder(outputDataHome+mouthEvidenceFolder);

    std::string ab_modelHome=projectHome+re_modelHome;
    ReadGlobalParamFromFile(ab_modelHome + featurePointsRegressorModelName);
    //initialize FaceAnalysisModel

    cv::Ptr<FaceAnalysisModel> pfam(new FaceAnalysisModel(ab_modelHome + fccModelName,ab_modelHome+pfccModelName, ab_modelHome + rightEyeStatusSVMModelName
                                                             , ab_modelHome + mouthChinStatusSVMModelName, ab_modelHome + featurePointsRegressorModelName,ab_modelHome+regName
                                                             , ab_modelHome + rightEyeDNNModelFile, ab_modelHome + rightEyeDNNWeightsFile
                                                             , ab_modelHome + rightEyeDNNMeanFile, ab_modelHome + faceComponentLabelFile
                                                             , ab_modelHome + mouthChinDNNModelFile, ab_modelHome + mouthChinDNNWeightsFile
                                                             , ab_modelHome + mouthChinDNNMeanFile, ab_modelHome + faceComponentLabelFile
                                                             , ab_modelHome + leftEyeDNNModelFile, ab_modelHome + leftEyeDNNWeightsFile
                                                             , ab_modelHome + leftEyeDNNMeanFile,ab_modelHome + faceComponentLabelFile
                                                             , ab_modelHome+headposeDNNModelFile,ab_modelHome+headposeDNNWeightsFile
                                                             , ab_modelHome+headposeDNNMeanFile,ab_modelHome+headposeLabelFile));

    //initialize FrameSequenceProcessor
    FrameSequenceProcessor fps(pfam
                               , outputDataHome+videoFolder
                               ,outputDataHome+eyesEvidenceFolder
                               ,outputDataHome+mouthEvidenceFolder);

    //intialize camera
    Camera camera(fps);

	camera.run();

	return 0;
}
