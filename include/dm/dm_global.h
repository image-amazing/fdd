#ifndef DM_GLOBAL_H
#define DM_GLOBAL_H

#include<string>
namespace dm{

struct DriverMonitorConfigure{
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
  std::string outputDataHome;
  std::string videoFolder;
  std::string eyeEvidenceFolder;
  std::string mouthEvidenceFolder;
  std::string resultFolder;
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
  std::string cameraID;
  std::string resultMessageQueueKey;
  std::string resultMessageProcessorPath;
};

DriverMonitorConfigure configGlobalVariables(const std::string &configFile);
int removeContents(const std::string &path);
void checkFolder(const std::string &folderName,bool bRetain=true);
}

#endif
