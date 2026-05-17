/*
    Jonathon Davis
    2026-05-17
    TODO Purpose of file
*/

#include <opencv2/opencv.hpp>

/*
    TODO Description of what it does

    @param src source image
    @param dst destination image
    @return error code, 0 on success, -1 on failure
*/
int greyscale(cv::Mat &src, cv::Mat &dst) {
    src.copyTo(dst);
    return 0;
}

/*
    Implement a 5x5 blur filter using at<> method using
    integer approximation of Gaussian:

    1  2  4  2  1
    2  4  8  4  2
    4  8  16 8  4
    2  4  8  4  2
    1  2  4  2  1

    @param src source image
    @param dst destination image
    @return error code, 0 on success, -1 on failure
*/
int blur5x5_1(cv::Mat &src, cv::Mat &dst) {
    src.copyTo(dst);

    for (int i = 2; i < src.rows - 2; i++) {
        for (int j = 2; j < src.cols - 2; j++) {
            for (int k = 0; k < 3; k++) {
                // TODO make this a loop?
                dst.at<cv::Vec3b>(i, j)[k] =
                    (src.at<cv::Vec3b>(i - 2, j - 2)[k] * 1 +
                     src.at<cv::Vec3b>(i - 2, j - 1)[k] * 2 +
                     src.at<cv::Vec3b>(i - 2, j)[k] * 4 +
                     src.at<cv::Vec3b>(i - 2, j + 1)[k] * 2 +
                     src.at<cv::Vec3b>(i - 2, j + 2)[k] * 1 +

                     src.at<cv::Vec3b>(i - 1, j - 1)[k] * 2 +
                     src.at<cv::Vec3b>(i - 1, j - 2)[k] * 4 +
                     src.at<cv::Vec3b>(i - 1, j)[k] * 8 +
                     src.at<cv::Vec3b>(i - 1, j + 1)[k] * 4 +
                     src.at<cv::Vec3b>(i - 1, j + 2)[k] * 2 +

                     src.at<cv::Vec3b>(i, j - 2)[k] * 4 +
                     src.at<cv::Vec3b>(i, j - 1)[k] * 8 +
                     src.at<cv::Vec3b>(i, j)[k] * 16 +
                     src.at<cv::Vec3b>(i, j + 1)[k] * 8 +
                     src.at<cv::Vec3b>(i, j + 2)[k] * 4 +

                     src.at<cv::Vec3b>(i + 1, j - 2)[k] * 2 +
                     src.at<cv::Vec3b>(i + 1, j - 1)[k] * 4 +
                     src.at<cv::Vec3b>(i + 1, j)[k] * 8 +
                     src.at<cv::Vec3b>(i + 1, j + 1)[k] * 4 +
                     src.at<cv::Vec3b>(i + 1, j + 2)[k] * 2 +

                     src.at<cv::Vec3b>(i + 2, j - 2)[k] * 1 +
                     src.at<cv::Vec3b>(i + 2, j - 1)[k] * 2 +
                     src.at<cv::Vec3b>(i + 2, j)[k] * 4 +
                     src.at<cv::Vec3b>(i + 2, j + 1)[k] * 2 +
                     src.at<cv::Vec3b>(i + 2, j + 2)[k] * 1) /
                    100;
            }
        }
    }
    return 0;
}
