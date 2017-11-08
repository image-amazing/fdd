#include<iostream>
#include"LBFRegressor.h"
#include"Camera.h"
#include"dm_global.h"
#include"FatigueDetectionFrameSequenceProcessor.h"
#include<assert.h>

int main(int argc,char *args[])
{
   assert(argc>1);
	configGlobalVariables(args[1]); 
    checkFolder(outputDataHome+videoFolder);
    checkFolder(outputDataHome+eyesEvidenceFolder);
    checkFolder(outputDataHome+mouthEvidenceFolder);

    std::string ab_modelHome=projectHome+re_modelHome;
    ReadGlobalParamFromFile(ab_modelHome + featurePointsRegressorModelName);

    //initialize FaceAnalysisModel
    cv::Ptr<FaceAnalysisModel> pfam(new FaceAnalysisModel());
    pfam->loadFCC(ab_modelHome + fccModelName);
    pfam->loadPFCC(ab_modelHome+pfccModelName);
    pfam->loadFeaturePointsRegressor(ab_modelHome + featurePointsRegressorModelName,ab_modelHome+regName);
    pfam->loadRightEyeStatusDNN(ab_modelHome + rightEyeDNNModelFile, ab_modelHome + rightEyeDNNWeightsFile
                                , ab_modelHome + rightEyeDNNMeanFile, ab_modelHome + faceComponentLabelFile);
    pfam->loadLeftEyeStatusDNN(ab_modelHome + leftEyeDNNModelFile, ab_modelHome + leftEyeDNNWeightsFile
                               , ab_modelHome + leftEyeDNNMeanFile,ab_modelHome + faceComponentLabelFile);
    pfam->loadMouthChinStatusDNN( ab_modelHome + mouthChinDNNModelFile, ab_modelHome + mouthChinDNNWeightsFile
                                  , ab_modelHome + mouthChinDNNMeanFile, ab_modelHome + faceComponentLabelFile);
    pfam->loadHeadposeDNN(ab_modelHome+headposeDNNModelFile,ab_modelHome+headposeDNNWeightsFile
                          , ab_modelHome+headposeDNNMeanFile,ab_modelHome+headposeLabelFile);

    //initialize FrameSequenceProcessor
    FatigueDetectionFrameSequenceProcessor fps(pfam
                               , outputDataHome+videoFolder
                               ,outputDataHome+eyesEvidenceFolder
                               ,outputDataHome+mouthEvidenceFolder);

    //intialize camera
    Camera camera(fps);
	camera.run();

	return 0;
}
