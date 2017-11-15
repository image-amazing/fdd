#ifndef LBF_DEMO_H
#define LBF_DEMO_H
#include<string>
#include<vector>
using namespace std;
int FaceDetectionAndAlignment(const char* inputname,string modelPath);
double TestModel (vector<string> testDataName,string modelPath,string dataPath,string cascadeName);
void TrainModel(vector<string> trainDataName,string modelPath,string dataPath,string cascadeName);
#endif // LBF_DEMO_H
