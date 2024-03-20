#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>

#include <algorithm> // for std::max_element
#include <string>    // for std::string

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
//   std::string classLabel;
  int classLabel;
  int blobNumber;
  cv::Point2f center;
  double area;
  double perimeter;
  double circularity;
  double aspectRatio;
  cv::Point2f axisOfInertia;
  double elongation; // Added field for elongation
  double rectangularity; // Added field for rectangularity

  // Constructor (optional)
  BlobData(int num, cv::Point2f c, double a, double p, double circ, double ar, cv::Point2f axis, double elong, double rect)
      : blobNumber(num), classLabel(num), center(c), area(a), perimeter(p), circularity(circ), aspectRatio(ar), axisOfInertia(axis), elongation(elong), rectangularity(rect) {}
};

// Function to store blob data
void storeBlobData(std::vector<BlobData> &blobInfo, int blobNumber,int classLabel, cv::Point2f center, double area, double perimeter, double circularity, double aspectRatio, cv::Point2f axisOfInertia, double elongation, double rectangularity) {
  BlobData newBlob(blobNumber, center, area, perimeter, circularity, aspectRatio, axisOfInertia, elongation, rectangularity);
  blobInfo.push_back(newBlob);
}

#include <cmath> // for std::abs, std::pow

double compareBlobs(const BlobData &blob1, const BlobData &blob2) {
  double totalWeight = 5.0; // Adjust weights as needed (sum should be 1)
  double score = 0.0;

//   // Compare area with a weight of 1
  double areaWeight = 1.0 / totalWeight;
  double areaDiff = std::abs(blob1.area - blob2.area);
  double areaNormDiff = areaDiff / (std::max(blob1.area, blob2.area) + 1e-10);
  score += (1.0 - areaNormDiff) * areaWeight;
//   printf("area %f\n",(1.0 - areaNormDiff) * areaWeight);

//   // Compare perimeter with a weight of 1
  double perimeterWeight = 1.0 / totalWeight;
  double perimeterDiff = std::abs(blob1.perimeter - blob2.perimeter);
  double perimeterNormDiff = perimeterDiff / (std::max(blob1.perimeter, blob2.perimeter) + 1e-10);
  score += (1.0 - perimeterNormDiff) * perimeterWeight;
// //   printf("perimeter %f\n",(1.0 - perimeterNormDiff) * perimeterWeight);

//   Compare circularity with a weight of 1
  double circularityWeight = 1.0 / totalWeight;
  double circularityDiff = std::abs(blob1.circularity - blob2.circularity);
  score += (1.0 - circularityDiff) * circularityWeight;

//   Compare aspect ratio with a weight of 1
  double aspectRatioWeight = 2.0 / totalWeight;
  double aspectRatioDiff = std::abs(blob1.aspectRatio - blob2.aspectRatio);
  score += (1.0 - aspectRatioDiff) * aspectRatioWeight;
//   printf("aspect ratio %f\n",(1.0 - aspectRatioDiff) * aspectRatioWeight);

  // Compare elongation (assuming major and minor axis stored in axisOfInertia)
//   double elongationWeight = 1.0 / totalWeight;
//   double majorAxis1 = std::abs(blob1.axisOfInertia.x);
//   double minorAxis1 = std::abs(blob1.axisOfInertia.y);
//   double elongation1 = std::max(majorAxis1, minorAxis1) / (std::min(majorAxis1, minorAxis1) + 1e-10);
//   double majorAxis2 = std::abs(blob2.axisOfInertia.x);
//   double minorAxis2 = std::abs(blob2.axisOfInertia.y);
//   double elongation2 = std::max(majorAxis2, minorAxis2) / (std::min(majorAxis2, minorAxis2) + 1e-10);
//   double elongationDiff = std::abs(elongation1 - elongation2);
//   score += (1.0 - elongationDiff) * elongationWeight;
//   printf("elongation %f\n",(1.0 - elongationDiff) * elongationWeight);

    // Compare rectangularity (assuming bounding rectangle information available)
//   double rectangularityWeight = 1.0 / totalWeight;
//   double rectangularity1 = blob1.rectangularity; // Assuming rectangularity is already calculated and stored
//   double rectangularity2 = blob2.rectangularity;
//   double rectangularityDiff = std::abs(rectangularity1 - rectangularity2);
//   score += (1.0 - rectangularityDiff) * rectangularityWeight;
//   printf("rectangularity %f\n",(1.0 - rectangularityDiff) * rectangularityWeight);
  


  // Normalize score to 0-100 range
  score *= 100.0;

  return score;
}

std::map<int, char> blobToGroup;              // Map blob number to group letter
std::unordered_map<char, int> categoryCounts; // Map category letter to count

// Function to categorize blobs based on similarity
void categorizeBlobs(std::vector<BlobData> &blobInfo, double threshold)
{
    for (int i = 0; i < blobInfo.size(); ++i)
    {
        for (int j = i + 1; j < blobInfo.size(); ++j)
        {
            // printf("num %d \n", blobInfo[j].blobNumber);
            double similarityScore = compareBlobs(blobInfo[i], blobInfo[j]);
            if (similarityScore > threshold)
            {
                if (!blobToGroup.count(blobInfo[i].classLabel))
                {
                    blobInfo[j].classLabel = blobInfo[i].classLabel; //TODO
                }
            }
        }
        // printf("\n");
    }
}

int main()
{

    /*
    // Turning on camera and stream video
    cv::VideoCapture cap(1); // On my laptop "0" is the built-in camera.
    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }

    cv::Mat frame;

    // Capture a picture from video feed
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "No frame captured?" << std::endl;
        return -1;
    }


    cv::namedWindow("Original", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original", 800, 600);
    cv::imshow("Original", frame);
    */

    // cv::Mat frame = cv::imread("DEMO_circle_fish_star_01.jpg",cv::IMREAD_COLOR);
    cv::Mat frame = cv::imread("DEMO_circle_fish_star_02.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("DEMO_components_02.png", cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("FYI_components_01.png",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("FYI_components_05.png",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("FYI_components_03.png",cv::IMREAD_COLOR);
    if (frame.empty())
    {
        std::cerr << "Error: Could not open or find the image!\n";
        return -1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Picture identification
    
    cv::Mat image_blurred_with_3x3_kernel;
    cv::GaussianBlur(frame, image_blurred_with_3x3_kernel, cv::Size(3, 3), 0);
    // Convert to HSV
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Define lower and upper bounds for the color range
    cv::Scalar lower(0, 0, 0);      // Lower bound (Hue, Saturation, Value)
    cv::Scalar upper(179, 50, 255); // Upper bound (Hue, Saturation, Value)

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

    // Remove tiny blobs
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25));
    cv::morphologyEx(frame, frame, cv::MORPH_OPEN, kernel);

    //////////////////////////////////////////
    // Join blobs that are close together
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30));
    cv::morphologyEx(frame, frame, cv::MORPH_CLOSE, kernel);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(frame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<BlobData> blobInfo;
    
    // cv::namedWindow("frame", cv::WINDOW_NORMAL);
    // cv::resizeWindow("frame", 800, 600);
    // cv::imshow("frame", frame);
    // cv::waitKey(0);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Connected component analysis
    // /*
    cv::Mat labels, stats, centroids;
    int numComponents = cv::connectedComponentsWithStats(frame, labels, stats, centroids,8);

    // Assign random color to each blob
    std::vector<cv::Vec3b> colors(numComponents);
    for (int i = 0; i < numComponents; ++i)
    {
        colors[i] = cv::Vec3b(rand() & 255, rand() & 255, rand() & 255);
    }

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
    // */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (size_t i = 0; i < contours.size(); i++) {
        // Calculate area, perimeter, etc.
        double area = cv::contourArea(contours[i]);
        double perimeter = cv::arcLength(contours[i], true); // Set true for closed contour
        
        // Moments
        cv::Moments moments = cv::moments(contours[i]);
        int centroidX = moments.m10/moments.m00;
        int centroidY = moments.m01/moments.m00;
        
        // Variance of x and y coordinates (used for aspect ratio)
        double varX = moments.m20 / moments.m00;
        double varY = moments.m02 / moments.m00;

        // Aspect ratio (assuming width > height)
        cv::RotatedRect rotatedRect = cv::minAreaRect(contours[i]); 
        // Calculate aspect ratio from rotated rectangle
        double width = rotatedRect.size.width;
        double height = rotatedRect.size.height;
        double aspectRatio = width > height ? width / height : height / width;  // Handle both width > height and vice versa
        
        // Compute the axis of rotation
        double theta = 0.5 * atan2(2 * moments.m11, moments.m20 - moments.m02);

        double circularity = (4.0 * 3.142 * area) / (perimeter * perimeter);
        
        double elongation=0;
        double rectangularity=0;
        
        const cv::Scalar color = cv::Scalar(0, 255, 0);  // Green color
        const int thickness = 2; 

        // Calculate additional moments if needed (e.g., central moments)
        // Hu moments for shape recognition can also be extracted using moments data
            
        storeBlobData(blobInfo, i, i, cv::Point2f(centroidX, centroidY), area, perimeter,
                circularity, aspectRatio, cv::Point2f(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta)),
                elongation, rectangularity);
                
        printf("num:%d, Centre of area: [%2d,%2d], Area: %f, Perimeter: %f, Circularity: %f, Aspect ratio: %f, Elongation: %f, Rectangularity: %f,  Axis of minimum inertia: [%2f,%2f], theta: %2f\n",
                i+1, centroidX, centroidY, area, perimeter, circularity, aspectRatio, elongation, rectangularity,
                centroidX - 150 * sin(theta), centroidY - 150 * cos(theta),theta*180/3.142);
  
        cv::circle(coloredImage, cv::Point(centroidX, centroidY), 20, cv::Scalar(0, 0, 255), -1);

        // Draw the axis of rotation on the image
        cv::Point2d axis(centroidX - 150 * sin(theta), centroidY - 150 * cos(theta));
        cv::line(coloredImage, cv::Point(centroidX, centroidY), axis, cv::Scalar(0, 255, 0), 8);


    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Blob analysis
    /*
    std::unordered_map<char, int> classCounts; // Map class label to count

    // Stat calculations
    for (int label = 1; label < numComponents; ++label)
    {
        int x = stats.at<int>(label, cv::CC_STAT_LEFT);
        int y = stats.at<int>(label, cv::CC_STAT_TOP);
        int width = stats.at<int>(label, cv::CC_STAT_WIDTH);
        int height = stats.at<int>(label, cv::CC_STAT_HEIGHT);

        // Centre of mass
        double centroidX = centroids.at<double>(label, 0);
        double centroidY = centroids.at<double>(label, 1);

        cv::circle(coloredImage, cv::Point(centroidX, centroidY), 20, cv::Scalar(0, 0, 255), -1);

        // Moment calculations
        double m00 = 0.0;            // Area
        double m10 = 0.0, m01 = 0.0; // For perimeter calculation
        double m20 = 0.0, m02 = 0.0, m11 = 0.0;
        for (int y = 0; y < frame.rows; ++y)
        {
            for (int x = 0; x < frame.cols; ++x)
            {
                if (labels.at<int>(y, x) == label)
                {
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
        double perimeter = 2 * sqrt(3.142 * m00); // Based on object area

        // Circularity (assuming 4 * pi * Area / (Perimeter^2))
        double circularity = (4.0 * 3.142 * m00) / (perimeter * perimeter);

        // Compute the axis of rotation
        double theta = 0.5 * atan2(2 * m11, m20 - m02);

        // Draw the axis of rotation on the image
        cv::Point2d axis(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta));
        cv::line(coloredImage, cv::Point(centroidX, centroidY), axis, cv::Scalar(0, 255, 0), 8);

         // Elongation
        double majorAxis = std::max(sqrt(m20), sqrt(m02)); // Assuming major axis is larger
        double minorAxis = std::min(sqrt(m20), sqrt(m02)); // Assuming minor axis is smaller
        double elongation = majorAxis / (minorAxis + 1e-10); // Avoid division by zero

        // Rectangularity (assuming bounding rectangle information available)
        // cv::Rect boundingRect; // Assuming you have access to bounding rectangle data
        // double width = boundingRect.width;
        // double height = boundingRect.height;
        double rectangularity = std::min(float(width) / float(height), float(height) / float(width)); // Ratio of smaller dimension to larger dimension

        storeBlobData(blobInfo, label, cv::Point2f(centroidX, centroidY), m00, perimeter,
                circularity, aspectRatio, cv::Point2f(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta)),
                elongation, rectangularity);

        // printf("Blob number: %d, Centre of area: [%2f,%2f], Area: %f, Perimeter: %f, Circularity: %f, Aspect ratio: %f, Elongation: %f, Rectangularity: %f,  Axis of minimum inertia: [%2f,%2f]\n",
        //         label, centroidX, centroidY, m00, perimeter, circularity, aspectRatio, elongation, rectangularity,
        //         centroidX - 150 * cos(theta), centroidY - 150 * sin(theta));
    }
    */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    double threshold = 80.0; // Set the similarity threshold (0 to 100)

    // Categorize and print results
    categorizeBlobs(blobInfo, threshold);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Print similarity table and add category labels to image 

    std::map<int, char> blobToGroup;
    if (blobInfo.size() < 2)
    {
        std::cout << "Not enough blobs found for comparison. Minimum 2 blobs required." << std::endl;
        return 0;
    }
    // Print table header
    printf("%-10s", "Blob"); // Blob number header (adjust width as needed)
    for (int i = 0; i < blobInfo.size(); ++i)
    {
        printf("%6d ", i + 1); // Column headers for each blob (adjust width as needed)
    }
    printf("\n");

    
    // Print similarity scores
    for (int i = 0; i < blobInfo.size(); ++i)
    {
        printf("%-4d  %-6c", blobInfo[i].blobNumber+1, 'A'  + blobInfo[i].classLabel); // Print blob number (adjust width as needed) //TODO
        // printf("%-4d  %-6c", i + 1, 'A' - 1 + blobInfo[i].blobNumber); // Print blob number (adjust width as needed)
        for (int j = 0; j < blobInfo.size(); ++j)
        {
            if (i != j)
            { // Skip comparing a blob to itself
                double similarityScore = compareBlobs(blobInfo[i], blobInfo[j]);
                printf("%6.2f ", similarityScore); // Print similarity score (adjust width and precision as needed)
            }
            else
            {
                printf("%6s ", "----"); // Print "----" for self-comparison
            }
        }
        printf("\n");

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Add category labels on the image
        // Number to display (adjust formatting as needed)
        std::string numberText = std::string(1, 'A'  + blobInfo[i].classLabel); // TODO
        // std::string numberText = std::string(1, 'A' - 1 + blobInfo[i].blobNumber); // TODO
        // std::string numberText = std::string(1, blobInfo[i].blobNumber);

        // Get font for text rendering (adjust font settings as needed)
        cv::HersheyFonts font = cv::FONT_HERSHEY_DUPLEX; // Choose font type
        double fontScale = 4.0;                          // Text size multiplier
        int fontThickness = 5;                           // Text line thickness

        // Size of the text for vertical placement
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(numberText, font, fontScale, fontThickness, &baseline);

        // Position the text underneath the object's centroid
        int textX = blobInfo[i].center.x - textSize.width / 2;
        int textY = blobInfo[i].center.y + textSize.height + 5 + 100; // Adjust spacing as needed

        // Add the text to the image
        cv::putText(coloredImage, numberText, cv::Point(textX, textY), font, fontScale, cv::Scalar(118, 185, 0), fontThickness);
    }
    
    printf("\n");
    printf("\n");

    // for (int i=2;i<4;i++){
    //     printf("%-4d  %-6c", i + 1, 'A'  + blobInfo[i].blobNumber); // Print blob number (adjust width as needed) //TODO
    //     // double similarityScore = compareBlobs(blobInfo[i], blobInfo[j]);
        
    //     printf("Blob number: %d, Centre of area: [%2d,%2d], Area: %f, Perimeter: %f, Circularity: %f, Aspect ratio: %f, Elongation: %f, Rectangularity: %f\n",
    //         i+1, blobInfo[i].center.x,blobInfo[i].center.y, blobInfo[i].area, blobInfo[i].perimeter, blobInfo[i].circularity, blobInfo[i].aspectRatio, blobInfo[i].elongation, blobInfo[i].rectangularity);
       
    // }
    // double similarityScore = compareBlobs(blobInfo[2], blobInfo[3]);
    // printf("%6.2f ", similarityScore); // Print similarity score (adjust width and precision as needed)

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Display image
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::resizeWindow("image", 800, 600);
    cv::imshow("image", coloredImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
