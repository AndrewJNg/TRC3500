include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
 
using namespace cv;
using namespace std;
 

double FindImageAngle(const Mat& image) {
    if (image.empty()) {
        cerr << "Image is empty\n";
        return -1; // Return a special value indicating an error
    }
 
    // Convert the image to grayscale if it is not already
    Mat grayImage;
    if (image.channels() == 3) {
        cvtColor(image, grayImage, COLOR_BGR2GRAY);
    } else {
        grayImage = image;
    }
 
    // Threshold to create a binary image
    Mat thresholdedImage;
    threshold(grayImage, thresholdedImage, 128, 255, THRESH_BINARY);
 
    // Find contours
    vector<vector<Point>> contours;
    findContours(thresholdedImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
 
    double maxArea = 0;
    vector<Point> largestContour;
    for (const auto& contour : contours) {
        double area = contourArea(contour);
        if (area > maxArea) {
            maxArea = area;
            largestContour = contour;
        }
    }
 
    // Calculate the moments and the angle of the minimum moment of inertia
    if (!largestContour.empty()) {
        Moments m = moments(largestContour);
        if (m.m00 != 0) {
            double angle = 0.5 * atan2(2 * m.mu11, m.mu20 - m.mu02) * 180 / CV_PI;
            return angle; // Return the calculated angle
        }
    } else {
        cout << "No significant blobs found\n";
        return 0; // Return zero if no blob is processed
    }
 
    return 0; // Return zero or another special value if no blob is processed
}
 

Mat rotateImage(const Mat& src, double angle) {
    // Get the center of the image
    Point2f center(src.cols / 2.0, src.rows / 2.0);
 
    // Get the rotation matrix with the center, the angle in degrees, and a scale factor
    Mat rot = getRotationMatrix2D(center, angle, 1.0);
 
    // Determine the bounding rectangle for the rotated image
    Rect2f bbox = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();
 
    // Adjust the rotation matrix to take into account the translation
    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
 
    // Perform the affine transformation
    Mat dst;
    warpAffine(src, dst, rot, bbox.size());
 
    return dst; // Return the rotated image
}
 

int main() {
 
    // Load an image from a file
    Mat image = imread("IMG_20240227_0004.jpg");
 
    // Calculate the angle of the image
    double angle = FindImageAngle(image);
 
    if (angle == -1) {
        cout << "Failed to process the image.\n";
    } else if (angle != 0) {
        cout << "Angle: " << angle << " degrees\n";
        // Rotate the image by the calculated angle
        Mat rotatedImage = rotateImage(image, angle);
 
        // Display the rotated image
        namedWindow("Rotated Image", WINDOW_AUTOSIZE);
        imshow("Rotated Image", rotatedImage);
        waitKey(0); // Wait for a key press
    } else {
        cout << "No significant blobs detected to calculate angle.\n";
    }
 
    return 0;
}