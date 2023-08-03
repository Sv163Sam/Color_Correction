#include "main.h"

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

int main()
{
    cv::Mat img1 = cv::imread("img/img_1.png");
    cv::Mat img2 = cv::imread("img/img_2.png");

    auto start_time = std::chrono::high_resolution_clock::now();

    //
    // Change img1, img2 position to change color addiction
    //
    color_alignment(img2, img1);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Execution time: " << duration_ms << " ms" << std::endl;

    imwrite("corrected_img/Cor_random1.png", img1);
    imwrite("corrected_img/Cor_random2.png", img2);

    return 0;
}