/*
   Jonathon Davis
   2026-05-15
   Purpose of the file
*/

#include <opencv2/opencv.hpp>

/*
    Description of what it does

    @param argc
    @param argv
    @return
*/
int main(int argc, char *argv[]) {
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

    // Comment explaining what the major code block does
    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        cv::imshow("Video", frame);

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
        if (key == 's') {
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
    }

    delete capdev;
    return (0);
}
