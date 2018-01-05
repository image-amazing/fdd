#include<iostream>
#include"LBFRegressor.h"
#include"Camera.h"
#include"dm_global.h"
#include"FatigueDetectionFrameSequenceProcessor.h"
#include<assert.h>
#include<stdio.h>
#include<sstream>

using namespace fdd;
using namespace LBF;
using namespace dm;

class CreateProcess{
private:
    FILE *fp_;
public:
    CreateProcess(const std::string &cmd)
        :fp_(NULL)
    {
            fp_=popen(cmd.c_str(),"r");
            assert(NULL!=fp_);
            char buf[256];
            while(true){
                fgets(buf,256,fp_);
                if(0==strcmp(buf,"good\n")){
                    break;
                }else if(0==strcmp(buf,"bad\n")){
                    std::cout<<"bad process :"<<cmd<<std::endl;
                    exit(-1);
                }else{
                    std::cout<<buf<<std::endl;
                }
            }
    }
    ~CreateProcess(){
            if(fp_){
                pclose(fp_);
            }
    }
};

int main(int argc,char *args[])
{
   assert(2<=argc);
   std::string configFilePath=args[1];
    DriverMonitorConfigure dmc = configGlobalVariables(configFilePath);

    checkFolder(dmc.outputDataHome);
    checkFolder(dmc.outputDataHome+dmc.videoFolder);
    checkFolder(dmc.outputDataHome+dmc.eyeEvidenceFolder);
    checkFolder(dmc.outputDataHome+dmc.mouthEvidenceFolder);
    checkFolder(dmc.outputDataHome+dmc.resultFolder);

    std::stringstream sout;
    sout<<dmc.resultMessageProcessorPath<<" "<<configFilePath;//<<" 2>&1 | tee processor.log ";
    std::string cmd=sout.str();
    CreateProcess createProcess(cmd);

    std::string ab_modelHome=dmc.projectHome+dmc.re_modelHome;
    ReadGlobalParamFromFile(ab_modelHome +dmc.featurePointsRegressorModelName);

    //initialize FaceAnalysisModel
    cv::Ptr<FaceAnalysisModel> pfam(new FaceAnalysisModel());
    pfam->loadFCC(ab_modelHome + dmc.fccModelName);
    pfam->loadPFCC(ab_modelHome+dmc.pfccModelName);
    pfam->loadFeaturePointsRegressor(ab_modelHome + dmc.featurePointsRegressorModelName,ab_modelHome+dmc.regName);
    pfam->loadRightEyeStatusDNN(ab_modelHome + dmc.rightEyeDNNModelFile, ab_modelHome + dmc.rightEyeDNNWeightsFile
                                , ab_modelHome + dmc.rightEyeDNNMeanFile, ab_modelHome + dmc.faceComponentLabelFile);
    pfam->loadLeftEyeStatusDNN(ab_modelHome +dmc.leftEyeDNNModelFile, ab_modelHome + dmc.leftEyeDNNWeightsFile
                               , ab_modelHome + dmc.leftEyeDNNMeanFile,ab_modelHome + dmc.faceComponentLabelFile);
    pfam->loadMouthChinStatusDNN( ab_modelHome + dmc.mouthChinDNNModelFile, ab_modelHome + dmc.mouthChinDNNWeightsFile
                                  , ab_modelHome + dmc.mouthChinDNNMeanFile, ab_modelHome + dmc.faceComponentLabelFile);
    pfam->loadHeadposeDNN(ab_modelHome+dmc.headposeDNNModelFile,ab_modelHome+dmc.headposeDNNWeightsFile
                          , ab_modelHome+dmc.headposeDNNMeanFile,ab_modelHome+dmc.headposeLabelFile);
    std::cout<<"right eye weights file: "<<ab_modelHome+dmc.rightEyeDNNWeightsFile<<std::endl;
    std::cout<<"left eye weights file: "<<ab_modelHome+dmc.leftEyeDNNWeightsFile<<std::endl;
    std::cout<<"mouth weights file: "<<ab_modelHome+dmc.mouthChinDNNWeightsFile<<std::endl;
    //initialize FrameSequenceProcessor
    FatigueDetectionFrameSequenceProcessor fps(pfam,static_cast<key_t>(atoi(dmc.resultMessageQueueKey.c_str())));
    fps.set_videoFolder( dmc.outputDataHome+dmc.videoFolder);
    fps.set_eyeEvidenceFolder(dmc.outputDataHome+dmc.eyeEvidenceFolder);
    fps.set_mouthEvidenceFolder(dmc.outputDataHome+dmc.mouthEvidenceFolder);
    fps.set_resultFolder(dmc.outputDataHome+dmc.resultFolder);

    //intialize camera
    int cameraId=0;
    if(2==argc){
        cameraId=atoi(dmc.cameraID.c_str());
    }else if(3==argc){
        cameraId=atoi(args[2]);
    }
    Camera camera(fps,cameraId);
	camera.run();

	return 0;
}
