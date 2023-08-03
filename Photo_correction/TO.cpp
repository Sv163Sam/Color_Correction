//
// Created by Владимир Скобцов on 03.08.2023.
//

/*
void color_correction(Mat& img, cv::Scalar &desired_balance)
{
    cv::Scalar diff = desired_balance - mean(img);

    for (int i = 0; i < img.rows ; i++)
    {
        for(int j = 0; j < img.cols ; j++)
        {
            cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
            pixel[0] += diff[0] * 1.5;
            pixel[1] += diff[1] * 1.5;
            pixel[2] += diff[2] * 1.5;
            img.at<cv::Vec3b>(i, j) = pixel;
        }
    }
}


Mat corrector(Mat& img)
{
    // Split image into R, G, B channels
    Mat channels[3];
    split(img, channels);

    // Calculate mean brightness for each channel
    double meanR = mean(channels[0])[0];
    double meanG = mean(channels[1])[0];
    double meanB = mean(channels[2])[0];

    // Calculate correction coefficients for each channel
    double coeffR = (meanR + meanG + meanB) / (3 * meanR);
    double coeffG = (meanR + meanG + meanB) / (3 * meanG);
    double coeffB = (meanR + meanG + meanB) / (3 * meanB);

    // Apply correction to each pixel in each channel
    channels[0] = channels[0] * coeffR;
    channels[1] = channels[1] * coeffG;
    channels[2] = channels[2] * coeffB;

    // Merge corrected channels back into image
    Mat correctedImg;
    merge(channels, 3, correctedImg);

    // Display original and corrected images
    return correctedImg;
}

int main()
{
    cv::Mat image1 = cv::imread("img/TEST.png");
    cv::Mat image2 = cv::imread("img/MEST.png");

    auto start_time = std::chrono::high_resolution_clock::now();

    cv::Scalar desiredBalance = (mean(image1) + mean(image2)) / 2;

    //color_correction(image1, desiredBalance);
    //color_correction(image2, desiredBalance);
    image1 = corrector(image1);
    image2 = corrector(image2);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Execution time: " << duration_ms << " ms" << std::endl;

    imwrite("corrected_img/cor_img1.png", image1);
    imwrite("corrected_img/cor_img2.png", image2);

    Mat img1 = imread("corrected_img/cor_img1.png");
    Mat img2 = imread("corrected_img/cor_img2.png");

    int mergedWidth = img1.cols + img2.cols;
    int mergedHeight = max(img1.rows, img2.rows);
    Mat mergedImg(mergedHeight, mergedWidth, CV_8UC3);

    vector<Mat> img1Channels, img2Channels;
    split(img1, img1Channels);
    split(img2, img2Channels);

    for (int i = 0; i < 3; i++)
    {
        Scalar img1Mean = mean(img1Channels[i]);
        Scalar img2Mean = mean(img2Channels[i]);

        double correctionFactor = img1Mean[0] / img2Mean[0];

        img2Channels[i] *= correctionFactor;

        merge(img1Channels, img1);
        merge(img2Channels, img2);
    }

    Mat left(mergedImg, Rect(0, 0, img1.cols, img1.rows));
    img1.copyTo(left);
    Mat right(mergedImg, Rect(img1.cols  , 0, img2.cols, img2.rows));
    img2.copyTo(right);

    imwrite("mergedImage2.png", mergedImg);

    return 0;
}
*/


/*
void alignColorBalance(Mat& img1, Mat& img2, Rect overlap)
{
    int block_size = 32;
    int rows = overlap.height / block_size;
    int cols = overlap.width / block_size;

    std::vector<Vec3d> means1(rows * cols);
    std::vector<Vec3d> means2(rows * cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Rect block(overlap.x + j * block_size, overlap.y + i * block_size, block_size, block_size);

            Scalar mean1 = mean(img1(block));
            Scalar mean2 = mean(img2(block));

            means1[i * cols + j] = Vec3d(mean1[0], mean1[1], mean1[2]);
            means2[i * cols + j] = Vec3d(mean2[0], mean2[1], mean2[2]);
        }
    }

    // Compute scaling factors for each channel
    Vec3d scale(1.0, 1.0, 1.0);
    double sum1 = 0.0, sum2 = 0.0;

    for (int i = 0; i < rows * cols; i++)
    {
        sum1 += means1[i][0] + means1[i][1] + means1[i][2];
        sum2 += means2[i][0] + means2[i][1] + means2[i][2];
    }
    scale[0] = scale[1] = scale[2] = sum1 / sum2;

    for (int i = overlap.y; i < overlap.y + overlap.height; i++)
    {
        auto* row_ptr = img2.ptr<Vec3b>(i);

        for (int j = overlap.x; j < overlap.x + overlap.width; j++)
        {
            Vec3b& pixel = row_ptr[j];
            pixel[0] = saturate_cast<uchar>(pixel[0] * scale[0]);
            pixel[1] = saturate_cast<uchar>(pixel[1] * scale[1]);
            pixel[2] = saturate_cast<uchar>(pixel[2] * scale[2]);
        }
    }
}

int main(int argc, char** argv)
{
    // Load two images
    Mat img1 = imread("img/img_1.png");
    Mat img2 = imread("img/img_2.png");

    auto start_time = std::chrono::high_resolution_clock::now();

    Rect overlap(0, 0, std::min(img1.cols, img2.cols), std::min(img1.rows, img2.rows));

    alignColorBalance(img1, img2, overlap);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Execution time: " << duration_ms << " ms" << std::endl;

    imwrite("дерево.png", img1);

    return 0;
}
*/


/*
#include <opencv2/opencv.hpp>

// Функция для выравнивания цвета между двумя изображениями в пространстве RGB32
void alignColor(cv::Mat& image1, cv::Mat& image2) {
    // Переводим изображения в пространство HSV
    cv::Mat hsv1, hsv2;
    cv::cvtColor(image1, hsv1, cv::COLOR_BGR2HSV);
    cv::cvtColor(image2, hsv2, cv::COLOR_BGR2HSV);

    // Вычисление среднего значения яркости в первом и втором изображении
    cv::Scalar meanBrightness1 = cv::mean(hsv1);
    cv::Scalar meanBrightness2 = cv::mean(hsv2);

    // Вычисление коэффициента масштабирования для яркости
    float scale = meanBrightness2[2] / meanBrightness1[2] ;

    // Масштабирование значения яркости в первом изображении
    hsv1 *= scale;

    // Переводим обратно изображения в пространство BGR
    cv::cvtColor(hsv1, image1, cv::COLOR_HSV2BGR);
}

int main() {
    // Загрузка двух изображений
    cv::Mat image1 = cv::imread("img/img_1.png");
    cv::Mat image2 = cv::imread("img/img_2.png");

    // Проверка, что изображения успешно загружены
    if (image1.empty() || image2.empty()) {
        std::cout << "Не удалось загрузить изображения!" << std::endl;
        return -1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    // Выравнивание цвета между изображениями
    alignColor(image2, image1);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Execution time: " << duration_ms << " ms" << std::endl;


    // Сохранение результатов
    cv::imwrite("aligned_image1.jpg", image1);
    cv::imwrite("aligned_image2.jpg", image2);

    return 0;
}
 */

/*
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Функция для выравнивания цвета, яркости и контраста двух изображений
void colorBrightnessContrastAlignment(Mat& image1, Mat& image2) {
    // Преобразование изображений в оттенки серого
    cvtColor(image1, image1, COLOR_BGR2RGB);
    cvtColor(image2, image2, COLOR_BGR2RGB);

    // Вычисление среднего значения яркости для каждого изображения
    Scalar mean1 = mean(image1);
    Scalar mean2 = mean(image2);

    // Вычисление коэффициентов для выравнивания яркости и контраста
    double alpha = mean2[0] / mean1[0];
    double beta = (mean2[0] - alpha * mean1[0]);

    // Применение выравнивания яркости и контраста к изображениям
    image1.convertTo(image1, -1, alpha, beta);
    image2.convertTo(image2, -1, alpha, beta);

    // Выравнивание цвета
    Ptr<xphoto::SimpleWB> wb = xphoto::createSimpleWB();
    wb->balanceWhite(image1, image1);
    wb->balanceWhite(image2, image2);
}

int main() {
    // Загрузка двух изображений
    Mat image1 = imread("img/img_1.png");
    Mat image2 = imread("img/img_2.png");

    // Проверка на успешную загрузку изображений
    if (image1.empty() || image2.empty()) {
        cout << "Ошибка при загрузке изображений" << endl;
        return -1;
    }

    // Выравнивание цвета, яркости и контраста
    colorBrightnessContrastAlignment(image1, image2);

    // Отображение результатов
    imshow("Image 1", image1);
    imshow("Image 2", image2);
    waitKey(0);

    return 0;

        cv::Scalar desiredBalance = (mean(image1) + mean(image2)) / 2;
}*/
