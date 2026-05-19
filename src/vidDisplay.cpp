/*
    Jonathon Davis
    2026-05-15
    Opens a video channel, creates a window, then loops capturing a new frame
   and displaying it in the current DisplayMode. Also displays frame timing.
*/

#include "../include/faceDetect.h"
#include "../include/filter.h"
#include <chrono>
#include <opencv2/opencv.hpp>

enum DisplayMode {
    kOriginal,
    kGreyscale,
    kAltGreyscale,
    kBlur5x5_1,
    kBlur5x5_2,
    kSepia,
    kSobelX3x3,
    kSobelY3x3,
    kGradientMagnitude,
    kBlurQuantize,
    kFaceDetect,
};

std::ostream &operator<<(std::ostream &os, DisplayMode d) {
    switch (d) {
    case DisplayMode::kOriginal:
        return os << "Original";
    case DisplayMode::kGreyscale:
        return os << "Greyscale";
    case DisplayMode::kAltGreyscale:
        return os << "Alternative Greyscale";
    case DisplayMode::kBlur5x5_1:
        return os << "Blur 5x5 1";
    case DisplayMode::kBlur5x5_2:
        return os << "Blur 5x5 2";
    case DisplayMode::kSepia:
        return os << "Sepia";
    case DisplayMode::kSobelX3x3:
        return os << "Sobel X 3x3";
    case DisplayMode::kSobelY3x3:
        return os << "Sobel Y 3x3";
    case DisplayMode::kGradientMagnitude:
        return os << "Gradient Magnitude";
    case DisplayMode::kBlurQuantize:
        return os << "Blur Quantize";
    case DisplayMode::kFaceDetect:
        return os << "Face Detect";
    default:
        return os << "Undefined";
    }
}

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

        std::chrono::time_point<std::chrono::steady_clock> start;
        std::chrono::time_point<std::chrono::steady_clock> end;
        // show image based upon current DisplayMode
        switch (displayMode) {
        case kOriginal: {
            cv::imshow("Video", frame);
            break;
        }
        case DisplayMode::kGreyscale: {
            cv::Mat greyscale_frame;
            start = std::chrono::high_resolution_clock::now();
            cv::cvtColor(frame, greyscale_frame, cv::COLOR_BGR2GRAY);
            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", greyscale_frame);
            break;
        }
        case DisplayMode::kAltGreyscale: {
            cv::Mat alt_greyscale_frame;
            start = std::chrono::high_resolution_clock::now();
            greyscale(frame, alt_greyscale_frame);
            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", alt_greyscale_frame);
            break;
        }
        case DisplayMode::kBlur5x5_1: {
            cv::Mat blur_5x5_1_frame;
            start = std::chrono::high_resolution_clock::now();
            blur5x5_1(frame, blur_5x5_1_frame);
            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", blur_5x5_1_frame);
            break;
        }
        case DisplayMode::kBlur5x5_2: {
            cv::Mat blur_5x5_2_frame;
            start = std::chrono::high_resolution_clock::now();
            blur5x5_2(frame, blur_5x5_2_frame);
            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", blur_5x5_2_frame);
            break;
        }
        case DisplayMode::kSepia: {
            cv::Mat sepia_frame;
            start = std::chrono::high_resolution_clock::now();
            sepia(frame, sepia_frame);
            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", sepia_frame);
            break;
        }
        case DisplayMode::kSobelX3x3: {
            cv::Mat sobel_x_3x3_output;
            start = std::chrono::high_resolution_clock::now();
            sobelX3x3(frame, sobel_x_3x3_output);
            end = std::chrono::high_resolution_clock::now();

            cv::Mat display_frame;
            cv::convertScaleAbs(sobel_x_3x3_output, display_frame);
            cv::imshow("Video", display_frame);
            break;
        }
        case DisplayMode::kSobelY3x3: {
            cv::Mat sobel_y_3x3_output;
            start = std::chrono::high_resolution_clock::now();
            sobelY3x3(frame, sobel_y_3x3_output);
            end = std::chrono::high_resolution_clock::now();

            cv::Mat display_frame;
            cv::convertScaleAbs(sobel_y_3x3_output, display_frame);
            cv::imshow("Video", display_frame);
            break;
        }
        case DisplayMode::kGradientMagnitude: {
            cv::Mat sobel_x_3x3_output;
            cv::Mat sobel_y_3x3_output;
            cv::Mat gradient_magnitude_frame;

            start = std::chrono::high_resolution_clock::now();
            sobelX3x3(frame, sobel_x_3x3_output);
            sobelY3x3(frame, sobel_y_3x3_output);
            magnitude(sobel_x_3x3_output, sobel_y_3x3_output,
                      gradient_magnitude_frame);
            end = std::chrono::high_resolution_clock::now();

            cv::imshow("Video", gradient_magnitude_frame);
            break;
        }
        case DisplayMode::kBlurQuantize: {
            cv::Mat blur_quantize_frame;
            start = std::chrono::high_resolution_clock::now();
            blurQuantize(frame, blur_quantize_frame, 10);
            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", blur_quantize_frame);
            break;
        }
        case DisplayMode::kFaceDetect: {
            start = std::chrono::high_resolution_clock::now();

            std::vector<cv::Rect> faces;
            cv::Mat grey;
            cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY, 0);
            detectFaces(grey, faces);

            drawBoxes(frame, faces);

            // add a little smoothing by averaging the last two detections
            cv::Rect last(0, 0, 0, 0);
            if (faces.size() > 0) {
                last.x = (faces[0].x + last.x) / 2;
                last.y = (faces[0].y + last.y) / 2;
                last.width = (faces[0].width + last.width) / 2;
                last.height = (faces[0].height + last.height) / 2;
            }

            end = std::chrono::high_resolution_clock::now();
            cv::imshow("Video", frame);
            break;
        }
        }
        std::chrono::duration<double, std::milli> elapsed = end - start;
        if (displayMode != kOriginal) {
            std::cout << displayMode << " took " << elapsed.count() << " ms\n";
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
        if (key == '1') { // blur 5x5 1 (slow, using at<> method)
            if (displayMode == kBlur5x5_1) {
                displayMode = kOriginal;
            } else {
                displayMode = kBlur5x5_1;
            }
        }
        if (key == '2') { // blur 5x5 2 (fast, using ptr method and separable)
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
        if (key == 'x') { // sobel X 3x3
            if (displayMode == kSobelX3x3) {
                displayMode = kOriginal;
            } else {
                displayMode = kSobelX3x3;
            }
        }
        if (key == 'y') { // sobel Y 3x3
            if (displayMode == kSobelY3x3) {
                displayMode = kOriginal;
            } else {
                displayMode = kSobelY3x3;
            }
        }
        if (key == 'm') { // gradient magnitude
            if (displayMode == kGradientMagnitude) {
                displayMode = kOriginal;
            } else {
                displayMode = kGradientMagnitude;
            }
        }
        if (key == 'b') { // blur quantize
            if (displayMode == kBlurQuantize) {
                displayMode = kOriginal;
            } else {
                displayMode = kBlurQuantize;
            }
        }
        if (key == 'f') { // face detect
            if (displayMode == kFaceDetect) {
                displayMode = kOriginal;
            } else {
                displayMode = kFaceDetect;
            }
        }
    }

    delete capdev;
    return (0);
}
