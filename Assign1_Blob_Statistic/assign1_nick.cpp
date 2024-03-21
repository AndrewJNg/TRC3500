#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>

#include <algorithm> // for std::max_element
#include <string>    // for std::string

#include <unordered_map> // for std::unordered_map
#include <cmath>


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
  double theta;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Blob data structure
  // Constructor (optional)
  BlobData(int num,double theta_in, cv::Point2f c, double a, double p, double circ, double ar, cv::Point2f axis, double elong, double rect)
      : blobNumber(num), classLabel(num), theta(theta_in), center(c), area(a), perimeter(p), circularity(circ), aspectRatio(ar), axisOfInertia(axis), elongation(elong), rectangularity(rect) {}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function blocks

// Store data function
void storeBlobData(std::vector<BlobData> &blobInfo, int blobNumber,int classLabel, double theta, cv::Point2f center, double area, double perimeter, double circularity, double aspectRatio, cv::Point2f axisOfInertia, double elongation, double rectangularity) {
  BlobData newBlob(blobNumber,theta, center, area, perimeter, circularity, aspectRatio, axisOfInertia, elongation, rectangularity);
  blobInfo.push_back(newBlob);
}

//////////////////////////////////////////////////
// Compare data function, outputs a similarity score from 0 to 100 (100 being identical)
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

//////////////////////////////////////////////////
// Categorize blobs function based on similarity
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
  // /*
    cv::VideoCapture cap(1);  // Replace with "0" for built-in camera if needed
    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    bool updateFrame = false;  // Flag to control frame update

    // Create a window to display the video
    cv::namedWindow("Original", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original", 800, 600);

    while (true) {
        // Capture a frame from the video feed
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "No frame captured?" << std::endl;
            return -1;
        }

        // Check for 't' key press
        int key = cv::waitKey(1);  // Wait for 1 millisecond for key input
        if (key == 't') {
          updateFrame = true;
        }

        // Update the displayed frame only when 't' is pressed
        if (updateFrame) {
            cv::imshow("Original", frame);
            updateFrame = false;  // Reset flag for next frame update
        }

        // Exit if any button is pressed
        if (key == 'q') {
            printf("Picture taken \n");
            break;
        }
    }
    // */
    
    

    /*
    // cv::Mat frame = cv::imread("DEMO_circle_fish_star_01.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("DEMO_circle_fish_star_02.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame = cv::imread("DEMO_components_02.png", cv::IMREAD_COLOR);
    cv::Mat frame = cv::imread("pic.jpg", cv::IMREAD_COLOR);

    if (frame.empty())
    {
        std::cerr << "Error: Could not open or find the image!\n";
        return -1;
    }
    */

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    // Apply thresholding to obtain binary image
    int threshold_type = cv::THRESH_BINARY + cv::THRESH_OTSU;
    cv::threshold(frame, frame, 0, 255, threshold_type);
    cv::bitwise_not(frame, frame);

    // Remove tiny blobs
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15,15)); // for fish 
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)); // for components
    cv::morphologyEx(frame, frame, cv::MORPH_OPEN, kernel);

    //////////////////////////////////////////
    // Join blobs that are close together
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(frame, frame, cv::MORPH_CLOSE, kernel);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(frame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<BlobData> blobInfo;

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
    // Blob analysis, computing each blob's staticstics

    for (size_t i = 0; i < contours.size(); i++) {
        // Calculate area & perimeter
        double area = cv::contourArea(contours[i]);
        double perimeter = cv::arcLength(contours[i], true); // Set true for closed contour
        
        // Calculate Moments
        cv::Moments moments = cv::moments(contours[i]);
        int centroidX = moments.m10/moments.m00;
        int centroidY = moments.m01/moments.m00;
        
        // Calculate Aspect ratio from rotated rectangle (for best fitting)
        cv::RotatedRect rotatedRect = cv::minAreaRect(contours[i]); 
        double width = rotatedRect.size.width;
        double height = rotatedRect.size.height;
        double aspectRatio = width > height ? width / height : height / width;  // Handle both width > height and vice versa
        
        // Compute the axis of rotation
        double theta = 0.5 * atan2(2 * (moments.m00*moments.m11 -moments.m10*moments.m01), (   (moments.m00*moments.m20-moments.m10*moments.m10) - (moments.m00*moments.m02-moments.m01*moments.m01)  ));

        // Compute circularity, elongation and rectangularity
        double circularity = (4.0 * 3.142 * area) / (perimeter * perimeter);
        double elongation=0; // decided not to use
        double rectangularity=0; // decided not to use
            
        // Store blob data into dynamic array    
        storeBlobData(blobInfo, i, i, theta,cv::Point2f(centroidX, centroidY), area, perimeter,
                circularity, aspectRatio, cv::Point2f(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta)),
                elongation, rectangularity);
                
        // printf("num:%d, Centre of area: [%2d,%2d], Area: %f, Perimeter: %f, Circularity: %f, Aspect ratio: %f, Elongation: %f, Rectangularity: %f,  Axis of minimum inertia: [%2f,%2f], theta: %2f\n",
        //         i+1, centroidX, centroidY, area, perimeter, circularity, aspectRatio, elongation, rectangularity,
        //         centroidX - 150 * sin(theta), centroidY - 150 * cos(theta),theta*180/3.142);
  
        // Draw circle on centroid point
        cv::circle(coloredImage, cv::Point(centroidX, centroidY), 5, cv::Scalar(0, 0, 255), -1);

        // Draw the axis of rotation on the image
        cv::Point2d axis(centroidX - 150 * cos(theta), centroidY - 150 * sin(theta));
        cv::line(coloredImage, cv::Point(centroidX, centroidY), axis, cv::Scalar(0, 255, 0), 3);


    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // apply similarity threshold to group blobs together
    double threshold = 70.0; // Set the similarity threshold (0 to 100)
    categorizeBlobs(blobInfo, threshold);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Print blob categories on image

    std::map<int, char> blobToGroup;
    for (int i = 0; i < blobInfo.size(); ++i)
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Similarity table
        // /*
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
        // */
        std::string blobText = std::string(1, 'A'  + blobInfo[i].classLabel); 

        // Get font for text rendering (adjust font settings as needed)
        cv::HersheyFonts font = cv::FONT_HERSHEY_DUPLEX; // Choose font type
        double fontScale = 1.0;                          // Text size multiplier
        int fontThickness = 1;                           // Text line thickness

        // Size of the text for vertical placement
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(blobText, font, fontScale, fontThickness, &baseline);

        // Position the text underneath the object's centroid
        int textX = blobInfo[i].center.x - textSize.width / 2;
        int textY = blobInfo[i].center.y + textSize.height + 5 + 10; // Adjust spacing as needed

        // Add blob labels
        cv::putText(coloredImage, blobText, cv::Point(textX, textY), font, fontScale, cv::Scalar(118, 185, 0), fontThickness);
    }
    printf("\n");
    printf("\n");

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Print blob classes and stats

    // // Use a map to store category and its count
    for (int i = 0; i < blobInfo.size(); ++i){
        printf("%-4d  %-6c", blobInfo[i].blobNumber+1, 'A'  + blobInfo[i].classLabel); // Print blob number (adjust width as needed) //TODO
        
        // printf("Centre of area: [%2d,%2d], Area: %f, Perimeter: %f, Circularity: %f, Aspect ratio: %f, Elongation: %f, Rectangularity: %f\n",
        //      blobInfo[i].center.x,`blobInfo[i].center.y, blobInfo[i].area, blobInfo[i].perimeter, blobInfo[i].circularity, blobInfo[i].aspectRatio, blobInfo[i].elongation, blobInfo[i].rectangularity);
             
        printf("Centre of area: [%2.0f,%2.0f], \t Axis of minimum moment of inertia: %f \n",
             blobInfo[i].center.x,blobInfo[i].center.y,blobInfo[i].theta*180/3.142);
        
        
        // Increment count for the current category
        categoryCounts['A'  + blobInfo[i].classLabel]++;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Print final categories and count
    printf("\n");
    printf("\nCategory Counts:\n");
    for (const auto& [category, count] : categoryCounts) {
        printf("%c: %d\n", category, count);
    }

    // double similarityScore = compareBlobs(blobInfo[2], blobInfo[3]);
    // printf("%6.2f ", similarityScore); // Print similarity score (adjust width and precision as needed)

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Display image
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::resizeWindow("image", 1280,720);
    cv::imshow("image", coloredImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
