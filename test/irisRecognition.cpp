#include "irisRecognition.h"
//#include "picornt.h"
#include <stdio.h>
#include "time.h"

#include <iostream>
#include <fstream>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace dlib;

int IrisRecognition::init(const char *model_eye, const char *model_iris){
	
	deserialize(model_eye) >>detector;
	deserialize(model_iris) >> sp;
	/*int size;
	FILE* file;
	file = fopen(eyes_cascade_xml, "rb");
	if(!file){
		printf("# cannot read cascade from '%s'\n", eyes_cascade_xml);
		return 1;
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	cascade = malloc(size);
	if(!cascade || size!=fread(cascade, 1, size, file))
		return 1;
	fclose(file);
	regressor.Load(model_iris);
    */
	
}

//fstream out("namelist.txt");


void IrisRecognition::buildFeaturePool(const char *dbPath){
	clock_t starttime = clock();
	std::vector<String> filenames = list_all_image_in_file(dbPath);
	Mat feature;
	
	for (size_t i = 0; i < filenames.size(); ++i){
		String filename = filenames[i];
		cout << i <<"  "<< filename << endl;
		getIrisFeature(filename.c_str(), feature);
		if (!feature.empty()) {
			db.imageName.push_back(filenames[i]);
			db.imageFeature.push_back(feature);
			
		}
		//else{
		//	out<<filename.c_str()<<endl;
		//}
	}
	
	//out.close();
	clock_t endtime = clock();
	
	cout << "Size of database :  " << db.imageName.size() << endl;
	cout << "Extract feature time: " << (double)(endtime - starttime) / CLOCKS_PER_SEC << endl;
	
}

bool myfunction(Point2f i, Point2f j){
	return (i.y < j.y);
}

RetrievalResult IrisRecognition::retrievalImage(const char *imagePath){

	Mat feature;
	RetrievalResult result;
	//clock_t a = clock();
	getIrisFeature(imagePath, feature);
	//clock_t b = clock();
	//cout << "detect time eveg img: " << (double)(b - a) / CLOCKS_PER_SEC << endl;
	if(feature.empty()){
		result.imagePath = "";
	 	result.score = 1.1;
		return result;
	}
	float dist;
	std::vector<Point2f> dists;
	//clock_t starttime = clock();
	for (size_t i = 0; i < db.imageName.size(); ++i){
		dist = getHammingDistance(db.imageFeature[i], feature, 1);
		Point2f pt(i, dist);
		dists.push_back(pt);
	}
	partial_sort(dists.begin(), dists.begin() + 1, dists.end(), myfunction);
	//clock_t endtime = clock();
	//cout << "retrievl eveg img: " << (double)(endtime - starttime) / CLOCKS_PER_SEC << endl;
	
	result.imagePath = db.imageName[int(dists[0].x)];
	result.score = dists[0].y;
	
	return result;
	
}


void IrisRecognition::getIrisFeature(const char *filename, cv::Mat &feature){
	
	//Mat Image = imread(filename, 0);
	
	Mat eyeImage;
	//bool detected = true;
	//if (detected)
	Mat ima = imread(filename, 0);
	int width_std = 280;
	if ( ima.cols > width_std )
	{
		cv::resize(ima,ima,cv::Size(width_std, (int)((float)(ima.rows*width_std/ima.cols))));	
	}
	
	std::vector<dlib::rectangle> dets;
	bool detected=detectEye(ima, dets,eyeImage);
	 if(!detected){
		 feature.data=NULL;
		 return;
	 }
	//cout<<dets.size()<<endl;
	//else
		//eyeImage = Image;
	
	
	
	
	//float scales = sqrt(102400.0 / (eyeImage.rows * eyeImage.cols));
    //resize(eyeImage, eyeImage, Size(), scales, scales);
	//cout<<"1"<<endl;
    //eyeImage = eyeImage.rowRange(20, 300);
	//cout<<"2"<<endl;
	std::vector<Point2d> irisContour, pupilContour;
	getIrisContour(ima,dets, irisContour, pupilContour,eyeImage);
	
	std::vector<double> irisCircle(3);
	std::vector<double> pupilCircle(3);
	fitCircle(irisContour, irisCircle);
	fitCircle(pupilContour, pupilCircle);
	
	Mat doubleEyeImage, polarImage;
	//cv::cvtColor(eyeImage, eyeImage, CV_RGB2GRAY);
	eyeImage.convertTo(doubleEyeImage, CV_32F);
	
	normaliseIris(doubleEyeImage, polarImage, irisCircle, pupilCircle, 20, 240);
	
	logGaborFeature(polarImage, feature, 18, 0.5);
	
	
	
	
	
	
	
	
}

bool IrisRecognition::detectEye(cv::Mat &ima, std::vector<dlib::rectangle> &dets,cv::Mat &eye){
	  //std::string imgg="./data/iris_image/Image/cq/left_0.jpg";
	 //Mat ima = imread(img, 1);
	 //int height=ima.rows/2;
	 //int width=ima.cols/2;
	 //Mat dst
	
	
	
     
     //imwrite("aa.jpg",ima);
	  //getchar();
	 //dlib::cv_image<bgr_pixel> image(ima);
	 array2d<uchar> image;
	 assign_image(image,dlib::cv_image<uchar>(ima));
	
     //load_image(image, img);
	 //long nr=image.nr()/2;
	 //long nc=image.nc()/2;
	 //image.set_size(nr,nc);
     //cout<<ima.rows<<endl;
     //cout<<ima.cols<<endl;     
     //pyramid_up(image);

     //cout<<"0"<<endl;      
	 clock_t starttime = clock();
     dets = detector(image);
	 clock_t endtime = clock();
	 cout << "detect time: " << (double)(endtime-starttime) / CLOCKS_PER_SEC << endl;
	// cout<<"1"<<endl;
	//cout<<"size:"<<dets.size()<<endl;
	
	 if(dets.size()==0){
		 //dlib::rectangle det=dlib::rectangle(17,400,75,880);
		 //dets.push_back(det);
		 return false;
	 }
	 //cout<<"2"<<endl;
	 //for(int i=0;i<dets.size();i++){
		int ru = dets[0].top();
		int rb = dets[0].bottom();
		int cl = dets[0].left();
		int cr = dets[0].right();
		//cout<<"4"<<endl;
		if(cr>=ima.cols){
			cr=ima.cols-1;
		}
		if(cl<=0){
			cl=1;
		}
	
		eye = ima.rowRange(ru, rb).colRange(cl, cr).clone();
	
		return true;
   //}
	 
	 
	
}


void IrisRecognition::logGaborFeature(const Mat &input, Mat &output, double minWaveLength, double sigmaOnf){
	int rows = input.rows;
	int cols = input.cols;

	int ndata = cols;
	// If there is an odd No of data points, throw away the last one.
	if (ndata % 2 == 1)
		ndata = ndata - 1;
	
	output = Mat(rows, 2*ndata, CV_8U);

	Mat logGabor(1, ndata, CV_32F, Scalar::all(0.0));
	// Initialize filter wavelength.
	double wavelength = minWaveLength;
	// Centre frequency of filter.
	double fo = 1.0 / wavelength;
	double r;
	for (size_t i = 0; i < ndata / 2 + 1; ++i){
		if (i != 0){
			r = double(i) / ndata;
			logGabor.ptr<float>(0)[i] = exp((-pow(log(r / fo), 2)) / (2 * pow(log(sigmaOnf), 2)));
		}
		else
			logGabor.ptr<float>(0)[i] = 0;
	}

	int m, n;
	Mat signal, padded, complexI;
	for (int i = 0; i < rows; i++){
		//cout<<"1"<<endl;
		signal = input.row(i).colRange(0, ndata);
        //cout<<"2"<<endl;
		m = getOptimalDFTSize(signal.rows);  // Return size of 2^x that suite for FFT
		n = getOptimalDFTSize(signal.cols);
		// Padding 0, result is @padded
		copyMakeBorder(signal, padded, 0, m - signal.rows, 0, n - signal.cols, BORDER_CONSTANT, Scalar::all(0));

		// Create planes to storage REAL part and IMAGE part, IMAGE part init are 0
		Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
		merge(planes, 2, complexI);

		dft(complexI, complexI);

		split(complexI, planes);
		planes[0] = planes[0].mul(logGabor);
		planes[1] = planes[1].mul(logGabor);
		merge(planes, 2, complexI);

		idft(complexI, complexI);
		
		split(complexI, planes);
		for (size_t j = 0; j < ndata; ++j){
			output.ptr<uchar>(i)[2 * j] = (planes[0].ptr<float>(0)[j] > 0 ? 255 : 0);
			output.ptr<uchar>(i)[2 * j + 1] = (planes[1].ptr<float>(0)[j] > 0 ? 255 : 0);
		}
	}

}

void IrisRecognition::normaliseIris(const Mat &eyeImage, Mat &polarImage, std::vector<double> irisCircle, std::vector<double> pupilCircle, int polarHeigh, int polarWidth){
	polarImage = Mat(polarHeigh, polarWidth, CV_32F, Scalar::all(0.0));
	
	int radiuspixels = polarHeigh + 2;
	int angledivisions = polarWidth - 1;
	
	double ox = pupilCircle[0] - irisCircle[0];
	double oy = pupilCircle[1] - irisCircle[1];
	double a = ox*ox + oy*oy;
	
	int sgn = 0;
	if (ox <= 0) sgn = -1;
	else sgn = 1;
	if (ox == 0 && oy > 0 ) sgn = 1;
	
	double phi;
	if (ox == 0) phi = CV_PI/2;
	else phi = atan(oy/ox);
	
	double theta, b, r, mul_r, xo, yo;
	for (size_t i = 0; i < polarWidth; ++i){
		theta = i * 2*CV_PI / angledivisions;
		b = sgn * cos(CV_PI - phi - theta);
		r = (sqrt(a) * b) + (sqrt(a * b * b - (a - irisCircle[2] * irisCircle[2])));
		r = r - pupilCircle[2];
		for (size_t j = 0; j < polarHeigh; ++j){
			mul_r = pupilCircle[2] + (j+1) * r / (radiuspixels - 1);
			xo = mul_r * cos(theta);
			yo = mul_r * sin(theta);
			xo = pupilCircle[0] + xo;
			yo = pupilCircle[1] - yo;
			
			polarImage.ptr<float>(j)[i] = bilinear(eyeImage, xo, yo);
			
		}
	}
	
	polarImage /= 255;

}

//void IrisRecognition::getIrisContour(const Mat &eyeImage, vector<Point2d> &irisCircle, vector<Point2d> &pupilCircle){
void IrisRecognition::getIrisContour(cv::Mat &ima, std::vector<dlib::rectangle> &dets, std::vector<Point2d> &irisCircle, std::vector<Point2d> &pupilCircle,const cv::Mat &eyeImage){
	
	int landmark_num = 20;
    /*int initial_number = 20;
	BoundingBox test_bounding_box;
	test_bounding_box.start_x = 0;
	test_bounding_box.start_y = 0;
	test_bounding_box.width = eyeImage.cols - 1;
	test_bounding_box.height = eyeImage.rows - 1;
	test_bounding_box.centroid_x = test_bounding_box.start_x + test_bounding_box.width/2.0;
	test_bounding_box.centroid_y = test_bounding_box.start_y + test_bounding_box.height/2.0; 

	Mat_<double> current_shape = regressor.Predict(eyeImage, test_bounding_box, initial_number);
	for(int i = 0;i < landmark_num;i++){
		if (i < 12)
			irisCircle.push_back(Point2d(current_shape(i,0),current_shape(i,1)));
		else
			pupilCircle.push_back(Point2d(current_shape(i,0),current_shape(i,1)));
	}	*/
	//Mat ima = imread(img, 1);
    //int height=ima.rows/2;
	//int width=ima.cols/2;
	//cv::resize(ima,ima,cv::Size(960,1080));
	//dlib::cv_image<bgr_pixel> image(ima);
    array2d<uchar> image;
	assign_image(image,dlib::cv_image<uchar>(ima));
    //load_image(image, img);
	//long nr=image.nr()/2;
	//long nc=image.nc()/2;
	//image.set_size(nr,nc);
            // Make the image larger so we can detect small faces.
    //pyramid_up(image);
	full_object_detection shape;
	//if(dets.size()==0){
		
		//dlib::rectangle det=dlib::rectangle(1,1,ima.cols-1,ima.rows-1);
		
	    //shape = sp(image, det);
		//cout<<"number"<<shape.num_parts()<<endl;
		
	//}
   // else
		shape = sp(image, dets[0]);
	for(int i = 0;i < landmark_num;i++){
		//<<"p1"<<endl;
		shape.part(i)(0)=shape.part(i)(0)-dets[0].left();
	
		shape.part(i)(1)=shape.part(i)(1)-dets[0].top();
		//cout<<shape.part(i)(0)<<":"<<shape.part(i)(1)<<endl;
		//cout<<"p2"<<endl;
		if (i < 12)
			irisCircle.push_back(Point2d(shape.part(i)(0),shape.part(i)(1)));
		else
			pupilCircle.push_back(Point2d(shape.part(i)(0),shape.part(i)(1)));
	}	
	/*
	Mat eyeImage_1 = eyeImage.clone();
	for(int i = 0;i < landmark_num;i++){
		circle(eyeImage_1,Point2d(shape.part(i)(0),shape.part(i)(1)),3,Scalar(255,0,0),-1,8,0);
	}
	imshow("result.jpg",eyeImage_1);
	waitKey(0);
	*/
	//imwrite("result.jpg", eyeImage_1);
	
	
}


float IrisRecognition::getHammingDistance(cv::Mat &feature1, cv::Mat &feature2, int scales){
	int nbits = feature1.cols;
	int wordsize = 8;
	int nwords = (nbits - 1) / wordsize + 1;
	
	int i, j;
	Mat hamFeature1(feature1.rows, nwords, CV_8U, Scalar::all(0));
	for (int r = 0; r < feature1.rows; ++r){
		for (int c = 0; c < feature1.cols; ++c){
			if (feature1.ptr<uchar>(r)[c]){
				i = c / wordsize;
				j = c % wordsize;
				hamFeature1.ptr<uchar>(r)[i] += pow(2, j);
			}
		}
	}
	
	HammingLUT lut;
	int c_shift, tmp, dist = feature1.rows * feature1.cols;
	for (int shifts = -8; shifts <= 8; ++shifts){
		Mat hamFeature2(feature2.rows, nwords, CV_8U, Scalar::all(0));
		for (int r = 0; r < feature2.rows; ++r){
			for (int c = 0; c < feature2.cols; ++c){
				c_shift = (c - 2 * scales * shifts + feature2.cols) % feature2.cols;
				if (feature2.ptr<uchar>(r)[c_shift]){
					i = c / wordsize;
					j = c % wordsize;
					hamFeature2.ptr<uchar>(r)[i] += pow(2, j);
				}
			}
		}
		
		tmp = lut(hamFeature1.data, hamFeature2.data, hamFeature1.rows * nwords);
		if (tmp < dist)
			dist = tmp;
	}
	
	return float(dist) / (feature1.rows * feature1.cols);
	
}

