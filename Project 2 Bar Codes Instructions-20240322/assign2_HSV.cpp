
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

    // cv::Mat frame = cv::imread("IMG_20240227_0002b.jpg",cv::IMREAD_COLOR);
    cv::Mat frame = cv::imread("IMG_20240227_0003.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("IMG_20240227_0004.jpg",cv::IMREAD_COLOR);

    if (frame.empty())
    {
        std::cerr << "Error: Could not open or find the image!\n";
        return -1;
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // [Option 1] Using Otsu's Binarization (https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html)
    // Mat greyImage;
    // Mat binaryImage1;

    // cv::cvtColor(frame, greyImage, cv::COLOR_BGR2GRAY);

    // int threshold_type =  cv::THRESH_BINARY + cv::THRESH_OTSU;
    // cv::threshold(greyImage, frame, 0, 255, threshold_type);
    // // // cv::bitwise_not(binaryImage1, binaryImage1); 
    // cv::imshow("Grey", frame);



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // /*
    // Picture identification (Using Hue, Saturation and Brightness value along with Otsu's Binarization, https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html)
    // Convert to HSV
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Define lower and upper bounds for the color range
    cv::Scalar lower(0, 0, 0);      // Lower bound (Hue, Saturation, Value)
    cv::Scalar upper(179, 30, 255); // Upper bound (Hue, Saturation, Value)

    // Threshold the HSV image to get only specified colors
    cv::Mat mask;
    cv::inRange(hsv, lower, upper, mask);

    // Apply the mask to the original image
    cv::Mat result;
    cv::bitwise_and(frame, frame, result, mask);

    // Convert the result back to grayscale
    cv::cvtColor(result, frame, cv::COLOR_BGR2GRAY);
    // */

    // Apply thresholding to obtain binary image
    // int threshold_type = cv::THRESH_BINARY + cv::THRESH_OTSU;
    // cv::threshold(frame, frame, 0, 255, threshold_type);
    // cv::bitwise_not(frame, frame);

    // Remove tiny blobs
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,1)); 
    // cv::morphologyEx(frame, frame, cv::MORPH_OPEN, kernel);

    //////////////////////////////////////////
    // Join blobs that are close together
    // kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    // cv::morphologyEx(frame, frame, cv::MORPH_CLOSE, kernel);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(frame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Colourise the blobs
    cv::Mat labels, stats, centroids;
    int numComponents = cv::connectedComponentsWithStats(frame, labels, stats, centroids,8);

    // Assign random color to each blob
    std::vector<cv::Vec3b> colors(numComponents);
    for (int i = 0; i < numComponents; ++i)  colors[i] = cv::Vec3b(rand() & 255, rand() & 255, rand() & 255);
    
    // Colorize the blobs
    cv::Mat coloredImage(frame.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    for (int y = 0; y < frame.rows; ++y)
    {
        for (int x = 0; x < frame.cols; ++x)
        {
            int label = labels.at<int>(y, x);
            if (label > 0)
            {
                coloredImage.at<cv::Vec3b>(y, x) = colors[label];
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    double max_area =0;
    
    // cv::Rect boundingRect;  // Store the rectangle coordinates
    cv::RotatedRect rotatedRect;


    for (size_t i = 0; i < contours.size(); i++) {
        // Calculate area & perimeter
        double area = cv::contourArea(contours[i]);
        if (area > max_area) 
        {
            max_area = area;
            // boundingRect = cv::boundingRect(contours[i]); 
            rotatedRect = cv::minAreaRect(contours[i]);
        }
    }
    
    // cv::rectangle(coloredImage, boundingRect, cv::Scalar(0, 255, 0), 2);  // Green rectangle with thickness 2    
    printf("max area: %f \n", max_area);
    
    // Get points of the rotated rectangle
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);
    // Find the top-left and bottom-right corners
    float x_min = vertices[0].x, y_min = vertices[0].y;
    float x_max = vertices[0].x, y_max = vertices[0].y;
    for (int i = 1; i < 4; i++) {
        x_min = std::min(x_min, vertices[i].x);
        y_min = std::min(y_min, vertices[i].y);
        x_max = std::max(x_max, vertices[i].x);
        y_max = std::max(y_max, vertices[i].y);
        
    }

     // Clamp bounding box coordinates
    x_min = std::max(0.0f, x_min);
    y_min = std::max(0.0f, y_min);
    x_max = std::min(frame.cols - 1.0f, x_max);
    y_max = std::min(frame.rows - 1.0f, y_max);

    // Define the bounding box
    cv::Rect boundingBox(cv::Point(std::round(x_min), std::round(y_min)),
                         cv::Size(std::round(x_max - x_min), std::round(y_max - y_min)));

    // Crop the original image using the bounding box
    cv::Mat croppedImage = frame(boundingBox);
    cv::rectangle(coloredImage, boundingBox, cv::Scalar(0, 255, 0), 2);  // Green rectangle with thickness 2  

    // Display the cropped image (optional)
    cv::imshow("Cropped Image", croppedImage);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Display image
    // cv::namedWindow("image", cv::WINDOW_NORMAL);
    // cv::resizeWindow("image", 1280,720);
    cv::imshow("image", coloredImage);
    // cv::imshow("image", binaryImage1);
    // cv::imshow("image", frame);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;

}