#pragma once
//#define CPU_ONLY
#define BOOST_ALL_DYN_LINK
#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include"caffe_layers_registry.hpp"
using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
//#pragma warning(disable:4099)
/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class CaffeClassifier {
public:
	CaffeClassifier(const string& model_file,
		const string& trained_file,
		const string& mean_file,
		const string& label_file);

	std::vector<Prediction> Classify(const cv::Mat& img, int N = 5);
	std::vector<float> Predict(const cv::Mat& img);
private:
	void SetMean(const string& mean_file);



	void WrapInputLayer(std::vector<cv::Mat>* input_channels);

	void Preprocess(const cv::Mat& img,
		std::vector<cv::Mat>* input_channels);

private:
	shared_ptr<Net<float> > net_;
	cv::Size input_geometry_;
	int num_channels_;
	cv::Mat mean_;
	std::vector<string> labels_;
};


