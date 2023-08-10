#include "main.h"

void color_correction_and_write(cv::Mat& img1, cv::Mat& img2, int overlap_width)
{
    //Calculate img borders
    int f_img_left_border = img1.cols - overlap_width;
    int f_img_right_border = img1.cols + overlap_width;
    //
    int s_img_left_border = 0;
    int s_img_right_border = overlap_width;
    //
    int img_bottom_height = 0;
    int img_top_height = img2.rows;
    //

    //Take img copy
    cv::Mat corrected_img = img1.clone();
    cv::Mat corrected_img2 = img2.clone();

    //Overlap elements cycle
    for (int y = img_bottom_height; y < img_top_height; y++)
    {
        for (int x = s_img_left_border; x < s_img_left_border + overlap_width; x++)
        {
            //Calculate position
            double dist_left = x - s_img_left_border;
            double dist_right = s_img_right_border - x;

            //Calculate pixel correction alpha, beta coef
            double alpha = dist_right / (dist_left + dist_right);
            double beta = dist_left / (dist_left + dist_right);

            //Take color-flow
            cv::Vec3b color_1 = img1.at<cv::Vec3b>(y, img1.cols - x);
            cv::Vec3b color_2 = img2.at<cv::Vec3b>(y, x - s_img_left_border);

            //Change color-flow
            cv::Vec3b new_color = alpha * color_1 + beta * color_2;

            //Paste color-flow
            corrected_img2.at<cv::Vec3b>(y, x) = new_color;
        }

        for (int x = f_img_left_border; x < f_img_left_border + overlap_width; x++)
        {
            double dist_left = x - f_img_left_border;
            double dist_right = f_img_right_border - x;

            double alpha = dist_right / (dist_left + dist_right);
            double beta = dist_left / (dist_left + dist_right);

            cv::Vec3b color_1 = img1.at<cv::Vec3b>(y, x);
            cv::Vec3b color_2 = img2.at<cv::Vec3b>(y, x - f_img_left_border);

            cv::Vec3b new_color = alpha * color_1 + beta * color_2;

            corrected_img.at<cv::Vec3b>(y, x) = new_color;
        }
    }

    imwrite("cor_stitch_imgs/Cor_img1.png", corrected_img);
    imwrite("cor_stitch_imgs/Cor_img2.png", corrected_img2);
}

void color_alignment(cv::Mat& img1, cv::Mat& img2)
{
    cv::Scalar mean1, mean2, stddev1, stddev2;

    // Calculate channels avg value
    meanStdDev(img1, mean1, stddev1);
    meanStdDev(img2, mean2, stddev2);

    // Color normalization
    for (int i = 0; i < img1.rows; i++) {
        for (int j = 0; j < img1.cols; j++) {
            auto& pixel1 = img1.at<cv::Vec3b>(i, j);
            pixel1[0] = cv::saturate_cast<uchar>((pixel1[0] - mean1[0]) * (stddev2[0] / stddev1[0]) + mean2[0]);
            pixel1[1] = cv::saturate_cast<uchar>((pixel1[1] - mean1[1]) * (stddev2[1] / stddev1[1]) + mean2[1]);
            pixel1[2] = cv::saturate_cast<uchar>((pixel1[2] - mean1[2]) * (stddev2[2] / stddev1[2]) + mean2[2]);
        }
    }
}

void start_color_alignment()
{
    cv::Mat img1 = cv::imread("img/1.png");
    cv::Mat img2 = cv::imread("img/2.png");

    auto start_time = std::chrono::high_resolution_clock::now();

    //
    // Change img1, img2 position to change color addiction
    //
    color_alignment(img1, img2);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Execution time: " << duration_ms << " ms" << std::endl;

    imwrite("corrected_img/Cor_random1.png", img1);
    imwrite("corrected_img/Cor_random2.png", img2);
}


void start_seam_correction()
{
    cv::Mat img1 = cv::imread("img/Green1.png");
    cv::Mat img2 = cv::imread("img/Green2.png");

    color_correction_and_write(img1, img2, o_width);
}
