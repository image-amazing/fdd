#pragma once
#include"Frame.h"
//#include"FaceAnalysisModel.h"
class FaceAnalysisModel;
class FaceComponent {
public:
	 enum Status {open,close};
protected:
	//原始帧
	Frame *pFrame_;
	//分类模型
	FaceAnalysisModel *pModel_;
	//组件的特征点坐标
	std::vector<cv::Point> featurePoints_;
	//组件的图像
	cv::Mat colorImg_;
	//组件图像放缩比例
	double colorImgScale_;
	//组件状态
	Status status_;
	//组件外接矩形的顶点
	cv::Point2f vertices_[4];
public:
	FaceComponent();
    FaceComponent(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel,double colorImgScale=1.0);
	virtual ~FaceComponent();
	//生成组件图像
	void generateColorImg();
	//模版方法
	FaceComponent::Status analyzeStatus();
	FaceComponent::Status status();
    void pFrame(const cv::Ptr<Frame> &pFrame);
    void pModel(const cv::Ptr<FaceAnalysisModel> &pModel);
	void featurePoints(std::vector<cv::Point> &featurePoints);
	void colorImgScale(double colorImgScale);
	//获得外接矩形顶点
	cv::Point2f * vertices();
	//绘制外接矩形
	void drawMinAreaRect();
private:
	//预测组件状态
	virtual FaceComponent::Status predictStatus() = 0;
};
