#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>

#include <algorithm>  // for std::max_element
#include <string>     // for std::string

#include <unordered_map> // for std::unordered_map
#include <unordered_map>
#include <cmath>

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


struct BlobData {
  std::string classLabel;
  int blobNumber;
  cv::Point2f center;
  double area;
  double perimeter;
  double circularity;
  double aspectRatio;
  cv::Point2f axisOfInertia;

  // Constructor (optional)
  BlobData(int num, cv::Point2f c, double a, double p, double circ, double ar, cv::Point2f axis) :
      blobNumber(num), center(c), area(a), perimeter(p), circularity(circ), aspectRatio(ar), axisOfInertia(axis) {}
};

// Function to store blob data
void storeBlobData(std::vector<BlobData>& blobInfo, int blobNumber, cv::Point2f center, double area, double perimeter, double circularity, double aspectRatio, cv::Point2f axisOfInertia) {
  BlobData newBlob(blobNumber, center, area, perimeter, circularity, aspectRatio, axisOfInertia);
  blobInfo.push_back(newBlob);
}

double compareBlobs(const BlobData& blob1, const BlobData& blob2) {
  double totalWeight = 3; // Adjust weights as needed (sum should be 1)
  double score = 0.0;

  // Compare area with a weight of 1
  double areaWeight = 1.0 / totalWeight;
  double areaDiff = std::abs(blob1.area - blob2.area);
  double areaNormDiff = areaDiff / (std::max(blob1.area, blob2.area) + 1e-10); // Avoid division by zero
  score += (1.0 - areaNormDiff) * areaWeight;

  // Compare perimeter with a weight of 1
  double perimeterWeight = 1.0 / totalWeight;
  double perimeterDiff = std::abs(blob1.perimeter - blob2.perimeter);
  double perimeterNormDiff = perimeterDiff / (std::max(blob1.perimeter, blob2.perimeter) + 1e-10);
  score += (1.0 - perimeterNormDiff) * perimeterWeight;

  // Compare circularity with a weight of 1
//   double circularityWeight = 1.0 / totalWeight;
//   double circularityDiff = std::abs(blob1.circularity - blob2.circularity);
//   score += (1.0 - circularityDiff) * circularityWeight;

  // Compare aspect ratio with a weight of 1
  double aspectRatioWeight = 1.0 / totalWeight;
  double aspectRatioDiff = std::abs(blob1.aspectRatio - blob2.aspectRatio);
  score += (1.0 - aspectRatioDiff) * aspectRatioWeight;

  // Normalize score to 0-100 range
  score *= 100.0;

  return score;
}


int nextClassLabel = 0; // Declare nextClassLabel

char assignUniqueLetterLabel(int& labelCounter) {
    char label = 'A' + labelCounter % 26;  // Use uppercase letters (wrap around at Z)
    labelCounter++;
    return label;
    }

int main() {
    
    /*
    // Turning on camera and stream video
    cv::VideoCapture cap(1); // On my laptop "0" is the built-in camera. 
    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }
    
    cv::Mat frame;

    // while(true){
    // Capture a picture from video feed
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "No frame captured?" << std::endl;
        return -1;
    }
    
    
    cv::namedWindow("Original", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original", 800, 600); 
    cv::imshow("Original", frame);

    // if ((char)cv::waitKey(0) > 0) break;
    
    */

    // cv::Mat frame = cv::imread("DEMO_circle_fish_star_02.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("DEMO_components_02.png",cv::IMREAD_COLOR);
    cv::Mat frame = cv::imread("FYI_components_01.png",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("FYI_components_05.png",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("FYI_components_03.png",cv::IMREAD_COLOR);
    if (frame.empty()) {
        std::cerr << "Error: Could not open or find the image!\n";
        return -1;
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Picture identification
    //Convert to HSV
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Define lower and upper bounds for the color range
    cv::Scalar lower(0, 0, 0); // Lower bound (Hue, Saturation, Value)
    cv::Scalar upper(255, 80, 255); // Upper bound (Hue, Saturation, Value)

    // Threshold the HSV image to get only specified colors
    cv::Mat mask;
    cv::inRange(hsv, lower, upper, mask);

    // Apply the mask to the original image
    cv::Mat result;
    cv::bitwise_and(frame, frame, result, mask);

    // Convert the result back to grayscale
    cv::cvtColor(result, frame, cv::COLOR_BGR2GRAY);

    // Apply thresholding to obtain binary image
    int threshold_type = cv::THRESH_BINARY + cv::THRESH_OTSU;
    cv::threshold(frame, frame, 0, 255, threshold_type);
    cv::bitwise_not(frame, frame);

    //Remove tiny blobs
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25));
    cv::morphologyEx(frame, frame, cv::MORPH_OPEN, kernel);

    //Join blobs that are close together
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30));
    cv::morphologyEx(frame, frame, cv::MORPH_CLOSE, kernel);

    // Connected component analysis
    cv::Mat labels, stats, centroids;
    int numComponents = cv::connectedComponentsWithStats(frame, labels, stats, centroids);

    // Assign random color to each blob
    std::vector<cv::Vec3b> colors(numComponents);
    for (int i = 0; i < numComponents; ++i) {
        colors[i] = cv::Vec3b(rand() & 255, rand() & 255, rand() & 255);
    }

    // Colorize the blobs
    cv::Mat coloredImage(frame.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    for (int y = 0; y < frame.rows; ++y) {
        for (int x = 0; x < frame.cols; ++x) {
            int label = labels.at<int>(y, x);
            if (label > 0) {
                coloredImage.at<cv::Vec3b>(y, x) = colors[label];
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Blob analysis
    // /*
    std::unordered_map<char, int> classCounts; // Map class label to count

    std::vector<BlobData> blobInfo;
    //Stat calculations
    for (int label = 1; label < numComponents; ++label) {
        //Centre of mass
        double centroidX = centroids.at<double>(label, 0);
        double centroidY = centroids.at<double>(label, 1);

        cv::circle(coloredImage, cv::Point(centroidX, centroidY), 20, cv::Scalar(0, 0, 255), -1);

        //Moment calculations
        double m00 = 0.0; // Area
        double m10 = 0.0, m01 = 0.0; // For perimeter calculation
        double m20 = 0.0, m02 = 0.0, m11 = 0.0;
        for (int y = 0; y < frame.rows; ++y) {
            for (int x = 0; x < frame.cols; ++x) {
                
                if (labels.at<int>(y, x) == label) {
                    m00++; // Area
                    m10 += x;
                    m01 += y;
                    m20 += (x - centroidX) * (x - centroidX);
                    m02 += (y - centroidY) * (y - centroidY);
                    m11 += (x - centroidX) * (y - centroidY);
                }
            }
        }


        // Variance of x and y coordinates (used for aspect ratio)
        double varX = m20 / m00;
        double varY = m02 / m00;

        // Aspect ratio (assuming width > height)
        double aspectRatio = sqrt(varX / varY);

        // Perimeter estimation (replace with more accurate method if needed)
        double perimeter = 2 * sqrt(3.142 * m00);  // Based on object area

        // Circularity (assuming 4 * pi * Area / (Perimeter^2))
        double circularity = (4.0 * 3.142 * m00) / (perimeter * perimeter);

        // Compute the axis of rotation
        double theta = 0.5 * atan2(2 * m11, m20 - m02);
        

        // Draw the axis of rotation on the image
        cv::Point2d axis(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta));
        cv::line(coloredImage, cv::Point(centroidX, centroidY), axis, cv::Scalar(0, 255, 0), 8);
        
        storeBlobData(blobInfo, label, cv::Point2f(centroidX, centroidY), m00, perimeter, circularity, aspectRatio, cv::Point2f(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta)));
        for (const BlobData& blob : blobInfo) {

        // Number to display (adjust formatting as needed)
        std::string numberText = std::to_string(blob.blobNumber);

        // Get font for text rendering (adjust font settings as needed)
        cv::HersheyFonts font = cv::FONT_HERSHEY_DUPLEX;  // Choose font type
        double fontScale = 4.0;  // Text size multiplier
        int fontThickness = 5;  // Text line thickness

        // Size of the text for vertical placement
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(numberText, font, fontScale, fontThickness, &baseline);

        // Position the text underneath the object's centroid
        int textX = blob.center.x - textSize.width / 2;
        int textY = blob.center.y + textSize.height + 5 + 100;  // Adjust spacing as needed

        // Add the text to the image
        cv::putText(coloredImage, numberText, cv::Point(textX, textY), font, fontScale, cv::Scalar(118, 185, 0), fontThickness);
        }
        
        // printf("Blob number: %d, Centre of area: [%2f,%2f], Area: %f, Perimeter: %f, Circularity: %f, Aspect ratio: %f, Axis of minimum inertia: [%2f,%2f]\n", label, centroidX, centroidY, m00, perimeter, circularity,aspectRatio, centroidX - 150 * cos(theta), centroidY - 150 * sin(theta));
        
    }
    // */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // print similarity table
    if (blobInfo.size() < 2) {
    std::cout << "Not enough blobs found for comparison. Minimum 2 blobs required." << std::endl;
    return 0;
    }
    // Print table header
    printf("%-10s", "Blob");  // Blob number header (adjust width as needed)
    for (int i = 0; i < blobInfo.size(); ++i) {
    printf("%6d ", i + 1);  // Column headers for each blob (adjust width as needed)
    }
    printf("\n");

    // Print similarity scores
    for (int i = 0; i < blobInfo.size(); ++i) {
    printf("%-10d", blobInfo[i].blobNumber);  // Print blob number (adjust width as needed)
    for (int j = 0; j < blobInfo.size(); ++j) {
        if (i != j) {  // Skip comparing a blob to itself
        double similarityScore = compareBlobs(blobInfo[i], blobInfo[j]);
        printf("%6.2f ", similarityScore);  // Print similarity score (adjust width and precision as needed)
        } else {
        printf("%6s ", "----");  // Print "----" for self-comparison
        }
    }
    printf("\n");
    }

    


    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Display image
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::resizeWindow("image", 800, 600); 
    cv::imshow("image", coloredImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
