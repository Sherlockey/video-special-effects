/*
    Jonathon Davis
    2026-05-15
    Opens a video channel, creates a window, then loops capturing a new frame
   and displaying it each time through the loop.
*/

#include "filter.cpp"
#include <opencv2/opencv.hpp>

enum DisplayMode {
    kOriginal,
    kGreyscale,
    kAltGreyscale,
    kBlur5x5_1,
    kBlur5x5_2,
    kSepia,
};

/*
    TODO Description of what it does

    @param argc argument count with how many command line arguments provided
    @param argv argument vector is an array of strings with command line args
    @return exit status / return code sent back to OS
*/
int main(int argc, char *argv[]) {
    DisplayMode displayMode = kOriginal;
    cv::VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return (-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;

    int saveCounter = 0; // used for image names
    std::string saveFolder = "./data/";

    // Main loop showing camera stream in current DisplayMode and handling input
    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        // show image based upon current DisplayMode
        switch (displayMode) {
        case kOriginal: {
            cv::imshow("Video", frame);
            break;
        }
        case kGreyscale: {
            cv::Mat greyscale_frame;
            cv::cvtColor(frame, greyscale_frame, cv::COLOR_BGR2GRAY);
            cv::imshow("Video", greyscale_frame);
            break;
        }
        case kAltGreyscale: {
            cv::Mat alt_greyscale_frame;
            greyscale(frame, alt_greyscale_frame);
            cv::imshow("Video", alt_greyscale_frame);
            break;
        }
        case kBlur5x5_1: {
            cv::Mat blur_5x5_1_frame;
            blur5x5_1(frame, blur_5x5_1_frame);
            cv::imshow("Video", blur_5x5_1_frame);
            break;
        }
        case kBlur5x5_2: {
            cv::Mat blur_5x5_2_frame;
            blur5x5_2(frame, blur_5x5_2_frame);
            cv::imshow("Video", blur_5x5_2_frame);
            break;
        }
        case kSepia: {
            cv::Mat sepia_frame;
            sepia(frame, sepia_frame);
            cv::imshow("Video", sepia_frame);
            break;
        }
        }

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'q') { // quit
            break;
        }
        if (key == 's') { // save
            std::string filename =
                saveFolder + std::to_string(saveCounter) + ".jpg";
            bool isSaved = cv::imwrite(filename, frame);
            if (isSaved) {
                std::cout << filename << " saved" << std::endl;
                saveCounter++;
            } else {
                std::cout << "failed to save image: " << filename << std::endl;
            }
        }
        if (key == 'g') { // greyscale
            if (displayMode == kGreyscale) {
                displayMode = kOriginal;
            } else {
                displayMode = kGreyscale;
            }
        }
        if (key == 'h') { // alternative greyscale
            if (displayMode == kAltGreyscale) {
                displayMode = kOriginal;
            } else {
                displayMode = kAltGreyscale;
            }
        }
        if (key == '1') { // blur5x5_1 (slow, using at<> method)
            if (displayMode == kBlur5x5_1) {
                displayMode = kOriginal;
            } else {
                displayMode = kBlur5x5_1;
            }
        }
        if (key == '2') { // blur5x5_2 (fast, using ptr method and separable)
            if (displayMode == kBlur5x5_2) {
                displayMode = kOriginal;
            } else {
                displayMode = kBlur5x5_2;
            }
        }
        if (key == 'p') { // sepia
            if (displayMode == kSepia) {
                displayMode = kOriginal;
            } else {
                displayMode = kSepia;
            }
        }
    }

    delete capdev;
    return (0);
}
