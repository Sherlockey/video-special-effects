/*
    Jonathon Davis
    2026-05-17
    Filters to be used with OpenCV cv::Mat data structures to modify the src.
*/

#include <opencv2/opencv.hpp>

/*
    TODO Description of what it does

    @param src source image
    @param dst destination image
    @return error code, 0 on success, -1 on failure
*/

int greyscale(cv::Mat &src, cv::Mat &dst) {
    // src.copyTo(dst);
    if (dst.empty()) {
        dst.create(src.rows, src.cols, CV_8UC1);
    }

    for (int i = 0; i < src.rows; i++) {
        cv::Vec3b *srcptr = src.ptr<cv::Vec3b>(i);
        cv::Vec<uchar, 1> *dstptr = dst.ptr<cv::Vec<uchar, 1>>(i);
        for (int j = 0; j < src.cols; j++) {
            short b = srcptr[j][0];
            short g = srcptr[j][1];
            short r = srcptr[j][2];

            // average method for greyscale
            // uchar y = (b + g + r) / 3;

            // lightness method for greyscale
            short max = std::max({b, g, r});
            short min = std::min({b, g, r});
            uchar y = (max + min) / 2;

            // max only method for greyscale
            // short max = std::max({b, g, r});
            // uchar y = max;

            // min only method for greyscale
            // short min = std::min({b, g, r});
            // uchar y = min;

            dstptr[j] = y;
        }
    }

    return 0;
}

/*
    Implement a 5x5 blur filter using at<> method using integer approximation of
   Gaussian:

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
    // valid convolution
    // leave outer row/col as the original image
    // allocates and copies the data to dst
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

/*
    Implement a 5x5 blur filter using separable 1x5 filters (vertical and
   horizontal) and the ptr method:

    1  2  4  2  1

    @param src source image
    @param dst destination image
    @return error code, 0 on success, -1 on failure
*/
int blur5x5_2(cv::Mat &src, cv::Mat &dst) {
    static cv::Mat tmp; // static for performance to avoid allocation overhead

    // allocate and initialize tmp
    src.copyTo(tmp);

    // run the horizontal 1 x 5 convolution [1 2 4 2 1]
    for (int i = 0; i < src.rows; i++) {
        cv::Vec3b *sptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *tptr = tmp.ptr<cv::Vec3b>(i);
        for (int j = 2; j < src.cols - 2; j++) {
            for (int k = 0; k < 3; k++) {
                tptr[j][k] = ((sptr[j - 2][k] * 1) + (sptr[j - 1][k] * 2) +
                              (sptr[j][k] * 4) + (sptr[j + 1][k] * 2) +
                              (sptr[j + 2][k] * 1)) /
                             10;
            }
        }
    }

    // allocate and initialize dst
    tmp.copyTo(dst);

    // run the vertical 3 x 1 convolution [1 2 4 2 1]^t
    for (int i = 2; i < src.rows - 2; i++) {
        cv::Vec3b *tptrm2 = tmp.ptr<cv::Vec3b>(i - 2);
        cv::Vec3b *tptrm1 = tmp.ptr<cv::Vec3b>(i - 1);
        cv::Vec3b *tptr = tmp.ptr<cv::Vec3b>(i);
        cv::Vec3b *tptrp1 = tmp.ptr<cv::Vec3b>(i + 1);
        cv::Vec3b *tptrp2 = tmp.ptr<cv::Vec3b>(i + 2);

        cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
        for (int j = 0; j < src.cols; j++) {
            for (int k = 0; k < 3; k++) {
                dptr[j][k] =
                    ((tptrm2[j][k] * 1) + (tptrm1[j][k] * 2) +
                     (tptr[j][k] * 4) + (tptrp1[j][k] * 2) + tptrp2[j][k] * 1) /
                    10;
            }
        }
    }

    return 0;
}
