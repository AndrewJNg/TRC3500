
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>


int main() {

        double value = 12.62;  // Your double value
        int dp_pos = 1;
        
        while(value<100)
        {
            value=value*10;
            dp_pos+=1;
        }
    printf("Value: %f \n",value);
    return 0;
}