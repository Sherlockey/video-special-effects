/*
   Jonathon Davis
   2026-05-15
   Read an image form a file and display it
*/

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string.h>

/*
    Reading an image given on the command line. Display the source image. Modify the image. Display the Image.

    @param argc
    @param argv
    @return
*/
int main(int argc, char* argv[])
{
    cv::Mat src; // the core image type, use it to store image from the file
    char filename[256];

    if (argc < 2) {
        printf("usage: %s <image filename>\n", argv[0]);
        exit(-1);
    }
    strncpy(filename, argv[1], 255);

    // try reading the image
    src = cv::imread(filename); // allocates the image, by default reads it in BGR format with 8-bits per channel

    // test if the read was successful
    if (src.data == NULL) {
        printf("error: unable to read image %s\n", filename);
        exit(-1);
    }

    // get the image size (height, width)
    printf("Image size:         %d rows %d columns\n", (int)src.rows, (int)src.cols);
    printf("Image size:         %d rows %d columns\n", (int)src.size().height, (int)src.size().width);
    printf("Image channels:     %d\n", (int)src.channels());
    printf("Bytes per channel:  %d\n", (int)src.elemSize() / src.channels());

    std::cout << "Bytes per channel: " << (int)(src.elemSize() / src.channels()) << std::endl;

    // create and display the image
    cv::namedWindow(filename, 1); // optional
    cv::moveWindow(filename, 0, 0);
    cv::imshow(filename, src); // imshow will create a new window, if necessary

    // modify the image using the at<> method to access pixels
    // swap the red and green channels in the image (BGR, swap position 1 with position 2 at each pixel)
    /*
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            // at is a templated function, tell it what data type to use
            // for an 8-bit per channel BGR image, cv::Vec3b (3 bytes per pixel), uchar or unsigned char
            uchar tmp = src.at<cv::Vec3b>(i, j)[1]; // green value at (i, j)
            src.at<cv::Vec3b>(i, j)[1] = src.at<cv::Vec3b>(i, j)[2]; // assign the red value to the green channel pos
            src.at<cv::Vec3b>(i, j)[2] = tmp; // assign the old green value to the red channel pos
        }
    }
    */

    // modify the image using the ptr<> method to access pixels
    for (int i = 0; i < src.rows; i++) {
        cv::Vec3b* ptr = src.ptr<cv::Vec3b>(i); // gets the address of the data for row i
        for (int j = 0; j < src.cols; j++) {
            char tmp = ptr[j][1]; // green value from column j
            ptr[j][1] = ptr[j][2]; // assign red value to the green position
            ptr[j][2] = tmp; // assign green value to the red position
        }
    }

    cv::namedWindow("Swap", 2);
    cv::moveWindow("Swap", src.cols + 10, 0);
    cv::imshow("Swap", src);

    cv::waitKey(0); // waits for a keypress before continuing (halting)

    return 0;
}
