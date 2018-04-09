#ifndef __IRISRECOGNITION_H
#define __IRISRECOGNITION_H

//#include "FaceAlignment.h"
#include "objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cv.h"
#include <dlib/svm_threaded.h>
#include <dlib/string.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/cmd_line_parser.h>
#include "utils.h"
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_io.h>
#include <iostream>
#include <fstream>

struct DbFeature {
	std::vector<cv::String> imageName;
    std::vector<cv::Mat> imageFeature;
};

struct RetrievalResult{
	cv::String imagePath;
	float score;
};


class IrisRecognition{
public:
	IrisRecognition(){
		init("./data/object_detector.svm", "./data/sp.dat");
	}
	~IrisRecognition(){
		
	}
	
	void buildFeaturePool(const char *dbPath);
	RetrievalResult retrievalImage(const char *imagePath);
	void getIrisFeature(const char *filename, cv::Mat &feature);
	float getHammingDistance(cv::Mat &feature1, cv::Mat &feature2, int scales);
	
private:
	int init(const char *model_eye, const char *model_iris);
	//void getIrisContour(const cv::Mat &eyeImage, std::vector<cv::Point2d> &irisCircle, std::vector<cv::Point2d> &pupilCircle);
	void getIrisContour(cv::Mat &ima, std::vector<dlib::rectangle> &dets, std::vector<cv::Point2d> &irisCircle, std::vector<cv::Point2d> &pupilCircle,const cv::Mat &eyeImage);
	void normaliseIris(const cv::Mat &eyeImage, cv::Mat &polarImage, std::vector<double> irisCircle, std::vector<double> pupilCircle,int polarHeigh, int polarWidth);
	void logGaborFeature(const cv::Mat &input, cv::Mat &output, double minWaveLength, double sigmaOnf);
	//bool detectEye(const cv::Mat &frame, cv::Mat &eye);
	bool detectEye(cv::Mat &ima, std::vector<dlib::rectangle> &dets,cv::Mat &eye);

	DbFeature db;
	//void *cascade = 0;
	typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;
	dlib::object_detector<image_scanner_type> detector;
	dlib::shape_predictor sp;
	//ShapeRegressor regressor;
};




#endif