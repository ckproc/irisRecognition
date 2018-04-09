#include <dirent.h>
#include <fstream>
#include <iterator>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils.h"

#include <iostream>

using namespace std;
using namespace cv;


static String sEndTime ="2018-7-1";


bool isMatch(cv::String s1, cv::String s2){
	int pos11 = s1.find("Image");
	int pos12 = s1.find('/', pos11 + 6);
	int pos21 = s2.find("Image");
	int pos22 = s2.find('/', pos21 + 6);
	
	return (s1.substr(pos11 + 6, pos12 - pos11 - 6) == s2.substr(pos21 + 6, pos22 - pos21 - 6));
	
}

double bilinear(const Mat &image, double x, double y){
	if (x < 0 || x > image.cols - 1 || y < 0 || y > image.rows - 1)
		return 0;
	double alpha = x - int(x);
	double beta = y - int(y);
	double lt = image.ptr<float>(int(y))[int(x)];
	double lb = image.ptr<float>(int(y) + 1)[int(x)];
	double rt = image.ptr<float>(int(y))[int(x) + 1];
	double rb = image.ptr<float>(int(y) + 1)[int(x) + 1];
	double value = (rt - lt) * alpha + (lb - lt) * beta + (rb + lt - lb - rt) * alpha * beta + lt;
	return value;
				
}

void fitCircle(const vector<Point2d> &points, vector<double> &circle){
    double X1, X2, X3, Y1, Y2, Y3, X1Y1, X1Y2, X2Y1;
    double C, D, E, G, H, N;
    double a, b,c;

    //拟合数据数量判断
    if (points.size() < 3){
        cerr << "Error: fit data number is less than 3!" << endl;;
    }

    X1 = X2 = X3 = Y1 = Y2 = Y3 = X1Y1 = X1Y2 = X2Y1 = 0;

    for (int i = 0; i < points.size(); i++){
        X1 = X1 + points[i].x;
        Y1 = Y1 + points[i].y;
        X2 = X2 + points[i].x * points[i].x;
        Y2 = Y2 + points[i].y * points[i].y;
        X3 = X3 + points[i].x * points[i].x * points[i].x;
        Y3 = Y3 + points[i].y * points[i].y * points[i].y;
        X1Y1 = X1Y1 + points[i].x * points[i].y;
        X1Y2 = X1Y2 + points[i].x * points[i].y * points[i].y;
        X2Y1 = X2Y1 + points[i].x * points[i].x * points[i].y;
    }

    N = points.size();
    C = N * X2 - X1 * X1;
    D = N * X1Y1 - X1 * Y1;
    E = N * X3 + N * X1Y2 - (X2 + Y2) * X1;
    G = N * Y2 - Y1 * Y1;
    H = N * X2Y1 + N * Y3 - (X2 + Y2) * Y1;

    a = (H * D-E * G) / (C * G - D * D);
    b = (H * C - E * D) / (D * D - G * C);
    c = -(a * X1 + b * Y1 + X2 + Y2)/N;

    circle[0] = a / (-2);
    circle[1] = b / (-2);
    circle[2] = sqrt(a * a + b * b - 4 * c) / 2;
}


vector<string> list_all_image_in_file(const char * images_txt) {
	vector<string> imagesList;

	ifstream myfile(images_txt);
	copy(istream_iterator<string>(myfile), istream_iterator<string>(), back_inserter(imagesList));

	return imagesList;
}

time_t str_to_time_t(const string& ATime, const string& AFormat="%d-%d-%d");
time_t NowTime();
bool IsValidTime(const time_t& AEndTime, const time_t& ANowTime );
bool isAvailability() {
    string sTemp;
    time_t t_Now = NowTime();
    time_t t_End = str_to_time_t(sEndTime);
    return IsValidTime(t_End, t_Now);
}

time_t str_to_time_t(const string& ATime, const string& AFormat)  {  
    struct tm tm_Temp;  
    time_t time_Ret;  
    try 
    {
        int i = sscanf(ATime.c_str(), AFormat.c_str(),// "%d/%d/%d %d:%d:%d" ,       
            &(tm_Temp.tm_year),   
            &(tm_Temp.tm_mon),   
            &(tm_Temp.tm_mday),  
            &(tm_Temp.tm_hour),  
            &(tm_Temp.tm_min),  
            &(tm_Temp.tm_sec),  
            &(tm_Temp.tm_wday),  
            &(tm_Temp.tm_yday));  
        tm_Temp.tm_year -= 1900;  
        tm_Temp.tm_mon --;  
        tm_Temp.tm_hour=0;  
        tm_Temp.tm_min=0;  
        tm_Temp.tm_sec=0;  
        tm_Temp.tm_isdst = 0;
        time_Ret = mktime(&tm_Temp);  
        return time_Ret;  
    } catch(...) {
        return 0;
    }
}  

time_t NowTime(){
    time_t t_Now = time(0);
    struct tm* tm_Now = localtime(&t_Now);
    tm_Now->tm_hour =0;
    tm_Now->tm_min = 0;
    tm_Now->tm_sec = 0;
    return  mktime(tm_Now);  
}

bool IsValidTime(const time_t& AEndTime, const time_t& ANowTime ){
    return (AEndTime >= ANowTime);
}