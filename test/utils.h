#ifndef __UTILS_H
#define __UTILS_H

#include <opencv/cv.h>
#include <opencv/highgui.h>


bool isMatch(cv::String s1, cv::String s2);
double bilinear(const cv::Mat &image, double x, double y);
void fitCircle(const std::vector<cv::Point2d> &points, std::vector<double> &circle);
std::vector<cv::String> list_all_image_in_file(const char *images_txt);
bool isAvailability();

#endif