#include<iostream>
#include<fstream>
#include"CaffeClassifier.h"
using namespace std;

const string root="/home/lyfe667/projects/test_headpose_model/";
const string modelHome="model/";
const string labelFile="headposeLabels.txt";
const string meanFile="headpose_mean.binaryproto";
const string modelFile="headpose_quick.prototxt";
const string weightsFile="headpose_snapshot__iter_6000.caffemodel";
const string pathFile="imgPaths.txt";

struct PathAndLabel{
    string path;
    int label;
};

int maxIndex(const vector<float>& probs){
    int maxIdx=0;
    for(int i=0;i<probs.size();i++){
            if(probs[maxIdx]<probs[i]){
                maxIdx=i;
            }
    }
    return maxIdx;
}

int main(){
    CaffeClassifier cc(root+modelHome+modelFile
                       ,root+modelHome+weightsFile
                       ,root+modelHome+meanFile
                       ,root+modelHome+labelFile);

    ifstream fin(root+pathFile);
    PathAndLabel pl;
    while(true){
        fin>>pl.path;
        if(fin.eof()){
            break;
        }
        cout<<pl.path<<endl;
        cv::Mat cImg=cv::imread(root+pl.path);
        vector<float> probs=cc.Predict(cImg);
        cout<<"predicted: "<<maxIndex(probs)<<endl;
        cv::imshow("cImg",cImg);
        cv::waitKey(0);
    }
    return 0;
}
