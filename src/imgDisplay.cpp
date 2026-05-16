/*
   Jonathon Davis
   2026-05-15
   Read an image from a file and display it
*/

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string.h>

/*
    Reading an image given on the command line. If q is pressed, quit.
    If s is pressed, save the image.

    @param argc argument count with how many command line arguments provided
    @param argv argument vector is an array of strings with command line args
    @return exit status / return code sent back to OS
*/

int main(int argc, char *argv[]) {
    // get command line arguments
    char filename[256];
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " <image filename>" << std::endl;
        exit(-1);
    }
    strncpy_s(filename, argv[1], 255);

    // try reading the image
    cv::Mat src;
    src = cv::imread(filename); // allocates the image, by default reads it in
                                // BGR format with 8-bits per channel

    // test if the read was successful
    if (src.data == NULL) {
        std::cout << "error: unable to read image" << filename << std::endl;
        exit(-1);
    }

    // create and display the image
    cv::namedWindow(filename, 1); //
    cv::moveWindow(filename, 0, 0);
    cv::imshow(filename, src);

    while (true) {
        char key = cv::waitKey(0);
        if (key == 'q') {
            break;
        }
        if (key == 's') {
            bool isSaved = cv::imwrite("./data/1.jpg", src);
            if (isSaved) {
                std::cout << "./data/1.jpg" << " saved" << std::endl;
            } else {
                std::cout << "failed to save image: " << "./data/1.jpg"
                          << std::endl;
            }
        }
    }

    return 0;
}
