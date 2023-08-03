#include "main.h"

using namespace cv;
using namespace std;

Mat color_correction(Mat& img, cv::Scalar &balance)
{
    // Split image into R, G, B channels
    Mat channels[3];
    split(img, channels);

    // Calculate mean brightness for each channel
    double mean_R = mean(channels[0])[0];
    double mean_G = mean(channels[1])[0];
    double mean_B = mean(channels[2])[0];

    // Calculate correction coefficients for each channel
    double coeff_R = (mean_R + mean_G + mean_B) / (1.5 * mean_R);
    double coeff_G = (mean_R + mean_G + mean_B) / (1.5 * mean_G);
    double coeff_B = (mean_R + mean_G + mean_B) / (1.5 * mean_B);

    // Apply correction to each pixel in each channel
    channels[0] = (channels[0] * coeff_R + balance[0])/2 ;
    channels[1] = (channels[1] * coeff_G + balance[1])/2 ;
    channels[2] = (channels[2] * coeff_B + balance[2])/2 ;

    // Merge corrected channels back into image
    Mat corrected_img;
    merge(channels, 3, corrected_img);

    // Display original and corrected images
    return corrected_img;
}

int main()
{

    //cv::Mat image1 = cv::imread("img/Ex1.png");
    //cv::Mat image2 = cv::imread("img/Ex2.png");

    //cv::Mat image1 = cv::imread("img/Green1.png");
    //cv::Mat image2 = cv::imread("img/Green2.png");

    cv::Mat image1 = cv::imread("img/img_1.png");
    cv::Mat image2 = cv::imread("img/img_2.png");

    //cv::Mat image1 = cv::imread("img/Street1.png");
    //cv::Mat image2 = cv::imread("img/Street2.png");

    auto start_time = std::chrono::high_resolution_clock::now();

    cv::Scalar desiredBalance = (mean(image1) + mean(image2)) / 2;

    image1 = color_correction(image1, desiredBalance);
    image2 = color_correction(image2, desiredBalance);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Execution time: " << duration_ms << " ms" << std::endl;

    imwrite("corrected_img/cor_img1.png", image1);
    imwrite("corrected_img/cor_img2.png", image2);

    return 0;
}