#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define o_width 100

void color_correction_and_write(cv::Mat& img1, cv::Mat& img2, int overlap_width);
void color_alignment(cv::Mat& img1, cv::Mat& img2);
void start_color_alignment();
void start_seam_correction();

#endif //MAIN_H
