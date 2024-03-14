// #include <opencv2/opencv.hpp>
// #include <iostrea
    /*
    // Turning on camera and stream video
    cv::VideoCapture cap(0); // On my laptop "0" is the built-in camera. 
    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }

    // Capture a picture from video feed
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "No frame captured?" << std::endl;
        return -1;
    }
    */m>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

  #include <opencv2/highgui/highgui.hpp>
 #include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

/* Ahmet 13-Feb-2024

Sample program for capturing and displaying a frame, and printing the pixel
values at a given position.   
To compile, copy the following lines into a file, say "compile.bat", and
save it in the directory where this file is stored. Then, run it in a Windows terminal.

  g++ -o opencv_cam opencv_cam.cpp -std=c++17^
    -I "C:\msys64\mingw64\include\opencv4"^
    -L "C:\msys64\mingw64\bin"^
    -lopencv_core-409 -lopencv_highgui-409 -lopencv_imgcodecs-409^
    -lopencv_imgproc-409 -lopencv_videoio-409
*/

using namespace cv;
int main() {

    /////////////// load an image and put it to frame (for troubleshooting without using camera to capture frame) //////////////////////////////
    // std::string image_path = samples::findFile("DEMO_circle_fish_star_01.jpg");
    // std::string image_path = samples::findFile("DEMO_circle_fish_star_02.jpg");
    // std::string image_path = samples::findFile("DEMO_components_02.png");
    std::string image_path = samples::findFile("FYI_components_01.png");
    // std::string image_path = samples::findFile("FYI_components_03.png");
    // std::string image_path = samples::findFile("FYI_components_04.png");
    // std::string image_path = samples::findFile("FYI_components_05.png");
    // std::string image_path = samples::findFile("FYI_components_06.png");
    
    Mat image = imread(image_path, IMREAD_COLOR);
    if(image.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    
    // Resize image
    cv::resize(image, image, cv::Size(1280,720));

    // Increase contrast
    // double alpha = 1.5; // Adjust for desired contrast level (higher for more contrast)
    // int beta = 0; // Adjust for brightness (positive for brighter, negative for darker)
    // cv::convertScaleAbs(image, image, alpha, beta);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // /*
    // [Option 1] Using Otsu's Binarization (https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html)

    // Convert to Greyscale
    Mat greyImage;
    cv::cvtColor(image, greyImage, cv::COLOR_BGR2GRAY);

    // Add threshold for black and white frame
    Mat binaryImage1;
    // int threshold_type = 0; // 0 for binary output (https://docs.opencv.org/4.x/db/d8e/tutorial_threshold.html#autotoc_md810)
    // threshold(greyFrame, binaryFrame, 175, 255, threshold_type ); 

    // adaptive threshold (https://docs.opencv.org/4.x/d7/dd0/tutorial_js_thresholding.html)
    // cv::adaptiveThreshold(greyImage,binaryImage, 200, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 2); 
    // cv::Mat binary_mask1;
    int threshold_type =  cv::THRESH_BINARY + cv::THRESH_OTSU;
    cv::threshold(greyImage, binaryImage1, 0, 255, threshold_type);
    cv::bitwise_not(binaryImage1, binaryImage1); 
    cv::imshow("Frame_threshold", binaryImage1);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(binaryImage1, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // */

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    // [Option 2] Using HSV spectrum to obtain binary image, so that it is less dependent on Brightness value    
    Mat hsv_img;
    cv::cvtColor(image,hsv_img,cv::COLOR_BGR2HSV);

    // cv::Scalar lower(18, 12, 31); // Lower bound (Hue, Saturation, Value)
    // cv::Scalar upper(112, 255, 255); // Upper bound (Hue, Saturation, Value)

    // cv::Scalar lower(0, 30, 50); // Lower bound (Hue, Saturation, Value)
    // cv::Scalar upper(179, 255, 255); // Upper bound (Hue, Saturation, Value)
    
    //--- Good setting if you only want the head of the component (not the legs) ---//
    cv::Scalar lower(0, 0, 55); // Lower bound (Hue, Saturation, Value)
    cv::Scalar upper(179, 40, 255); // Upper bound (Hue, Saturation, Value)
    

    cv::Mat mask;
    cv::inRange(hsv_img, lower, upper, mask);
    cv::bitwise_not(mask,mask);
    
    cv::Mat binary_mask;
    cv::threshold(mask, binary_mask, 127, 255, cv::THRESH_BINARY); // Adjust threshold value if needed

    cv::imshow("Frame_threshold", binary_mask);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(binary_mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    */

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int minimum_area_threshold = 250;

    // Analyze each contour (component)
    for (size_t i = 0; i < contours.size(); i++) {
        // Calculate area, perimeter, etc.
        double area = cv::contourArea(contours[i]);
        double perimeter = cv::arcLength(contours[i], true); // Set true for closed contour

        // Filter components based on size (optional)
        if (area > minimum_area_threshold) { // Set a minimum area threshold

        // Draw a bounding rectangle around the component
        cv::Rect rect = cv::boundingRect(contours[i]);
        cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 2); // Green rectangle

        // Calculate other properties (optional)
        cv::Moments moments = cv::moments(contours[i]);
        // cv::Point center = cv::momentCentroids(moments);

        // ... perform further analysis based on properties
        }
    }

    // Display the original image with bounding boxes
    cv::imshow("Original Image (Components Marked)", image);
    cv::waitKey(0);


    int x = 100; int y = 200;  // I am interested in this pixel 
    // cv::imshow("Frame_threshold", binaryFrame2);

    // cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);
    // std::cout << "Pixel values at (" << x << ", " << y << "): ";
    // std::cout << "B: " << static_cast<int>(pixel[0]) << " ";
    // std::cout << "G: " << static_cast<int>(pixel[1]) << " ";
    // std::cout << "R: " << static_cast<int>(pixel[2]) << std::endl;

    cv::waitKey(0);
    

    // cap.release();
    cv::destroyAllWindows();
    
    return 0;
} // main()



