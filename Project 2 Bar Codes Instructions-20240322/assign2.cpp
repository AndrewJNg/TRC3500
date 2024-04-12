
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

std::string ink_decoder(double T1, double T2, double T4, double T);
int inkSpreading(double val);
void analyse(const std::vector<int>& consecutive_counts, int i, bool dir_left, std::vector<char>& output);
std::vector<int> analyze_consecutive_occurrences(const std::vector<int>& data);
int isGuardBit(std::vector<int>& consecutive_counts, int i);
int left_digit(std::list<std::vector<char>>& list);
void printOutputList(const std::list<std::vector<char>>& output_list);
void print_list_of_output_lists(const std::list<std::list<std::vector<char>>>& list_of_output_lists);
std::list<std::pair<std::list<std::vector<char>>, int>>  scan_barcode(cv::Mat img);


using namespace cv;
int main() {

    // cv::Mat img = cv::imread("IMG_20240227_0002b.jpg",cv::IMREAD_COLOR);
    cv::Mat img = cv::imread("IMG_20240227_0003.jpg",cv::IMREAD_COLOR);
    // cv::Mat img = cv::imread("IMG_20240227_0004.jpg",cv::IMREAD_COLOR);
    // cv::Mat img = cv::imread("IMG_20240227_0005.jpg",cv::IMREAD_COLOR); 
    // cv::Mat img = cv::imread("IMG_20240227_0006.jpg",cv::IMREAD_COLOR);
    // cv::Mat img = cv::imread("IMG_20240227_0007.jpg",cv::IMREAD_COLOR); // problem
    // cv::Mat img = cv::imread("IMG_20240227_0008.jpg",cv::IMREAD_COLOR);
    // cv::Mat img = cv::imread("IMG_20240227_0009.jpg",cv::IMREAD_COLOR);
    // cv::Mat img = cv::imread("IMG_20240227_0010.jpg",cv::IMREAD_COLOR);

    if (img.empty())
    {
        std::cerr << "Error: Could not open or find the image!\n";
        return -1;
    }

    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

    cv::Mat blur;
    cv::GaussianBlur(gray_img, blur, cv::Size(5, 5), 0);

    cv::Mat binary_img;
    int threshold_type = cv::THRESH_BINARY + cv::THRESH_OTSU;
    cv::threshold(blur, binary_img, 0, 255, threshold_type);
    
    cv::Mat binary_img_flip;
    flip(binary_img, binary_img_flip, 1); // 1: Flip horizontally

    std::list<std::pair<std::list<std::vector<char>>, int>> result = scan_barcode(binary_img);
    std::list<std::pair<std::list<std::vector<char>>, int>> result_flip = scan_barcode(binary_img_flip);
    
    int count = 0;
    int count_flip = 0;
    std::list<std::vector<char>>  max_count_ori;
    std::list<std::vector<char>>  max_count_flip;

    for (const auto& count_pair : result){
        count = count_pair.second;
        max_count_ori = count_pair.first;
    }
    for (const auto& count_pair : result_flip){
        count_flip = count_pair.second;
        max_count_flip = count_pair.first;
    }

    // Print decoded barcode signal that occurs the most
    if(count > count_flip) printOutputList(max_count_ori);
    else printOutputList(max_count_flip);


    cv::imshow("image",binary_img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;

}


std::list<std::pair<std::list<std::vector<char>>, int>>  scan_barcode(cv::Mat img){
    
    std::list<std::list<std::vector<char>>> list_of_output_lists;
    std::list<std::vector<char>> max_output_list;
    
   for (int row_count = 0; row_count < img.rows; row_count++) {
        cv::Mat row_pixels = img.row(row_count);

        std::vector<int> consecutive_counts = analyze_consecutive_occurrences(row_pixels);

        // Print out the consecutive_counts vector
        // std::cout << "Consecutive Counts: ";
        // for (int count : consecutive_counts) {
        //     std::cout << count << " ";
        // }
        // std::cout << std::endl;

        
        if (consecutive_counts.size() >= (6 * 4 + 34)) {
            std::list<std::vector<char>> output_list;
            for (int i = 0; i < consecutive_counts.size() - 55; i++) {
                std::vector<int> count = {i, 26 + i, 27 + i, 28 + i, 29 + i, 56 + i};
                int guardSum = 0;

                for (int j : count) {
                    if (j < (6 * 4 + 33)) {
                        guardSum += isGuardBit(consecutive_counts, j);
                    }
                }

                if (guardSum >= 2) {
                    for (int j = i + 3; j < 6 * 4 + i; j += 4) {
                        std::vector<char> char_vector_result;
                        analyse(consecutive_counts, j, true, char_vector_result);
                        output_list.push_back(std::move(char_vector_result));
                    }

                    for (int j = i + 32; j < 6 * 4 + i + 32; j += 4) {
                        std::vector<char> char_vector_result;
                        analyse(consecutive_counts, j, false, char_vector_result);
                        output_list.push_back(std::move(char_vector_result));
                    }
                    break;
                }
            }

            int empty_element = false;
            for (const auto& vec : output_list) {
                if (vec.empty()) {
                empty_element= true;
                }
            }

        ///////////////////////////////////////////////////////////////////////////////////////////
        if(empty_element==false){
        
            int first_digit = left_digit(output_list);
            
            if (first_digit != -1) {
                std::vector<char> first_digit_vector = {static_cast<char>('0' + first_digit)};
                output_list.push_front(first_digit_vector);

                int odd_sum = 0;
                int even_sum = 0;
                    // printOutputList(output_list);
                
                /////////////////////////////////////////////////
                // Calculate odd_sum
                for (auto it = std::next(output_list.begin(), 1); it != output_list.end(); std::advance(it, 2)) {
                    odd_sum += (*it)[1] - '0'; // Convert char to int
                    // printf("%d\n",odd_sum);
                }
                
                // Calculate even_sum
                even_sum += first_digit;
                for (auto it = std::next(output_list.begin(), 2); std::distance(it, output_list.end()) > 2; std::advance(it, 2)) {
                    even_sum += (*it)[1] - '0'; // Convert char to int
                }

                // Calculate parity
                int parity =  ((odd_sum * 3 + even_sum) % 10 != 0) ? 10 - ((odd_sum * 3 + even_sum) % 10) : 0;
                // printf("%d, %d, %d\n", odd_sum,even_sum,parity);

                
                if (parity == output_list.back()[1] - '0') {
                    // printOutputList(output_list);
                    list_of_output_lists.push_back(output_list);
                }



                    
                
            }
            }
        ///////////////////////////////////////////////////////////////////////////////////////////   
        }   
    }

    
    ///////////////////////////////////////////////////////////////////////////////////////////
    std::list<std::pair<std::list<std::vector<char>>, int>> occurrence_counts;


    for (const auto& outer_list : list_of_output_lists) {
        bool found = false;
        // Create a copy of the current output list
        std::list<std::vector<char>> current_list(outer_list.begin(), outer_list.end());

        // Check if the list already exists in the list of counts
        for (auto& count_pair : occurrence_counts) {
        if (count_pair.first == current_list) {
            count_pair.second++; // Increment count if found
            // printf("count: %d\n",  count_pair.second);
            found = true;
            break;
        }
        }

        // If not found, add a new entry with count 1
        if (!found) {
        occurrence_counts.push_back({std::move(current_list), 1});
        }
    }
    



    int max_count = 0;
    std::list<std::pair<std::list<std::vector<char>>, int>> max_count_lists;

    // Iterate through occurrence_counts to find the maximum count
    for (const auto& count_pair : occurrence_counts) {
        if (count_pair.second > max_count) {
        max_count = count_pair.second;
        max_count_lists.clear(); // Clear previous entries with lower count
        max_count_lists.push_back(count_pair);
        } else if (count_pair.second == max_count) {
        // If another list has the same max count, add it to the list
        max_count_lists.push_back(count_pair);
        }
    }

    // return max_count_lists;
    for (const auto& count_pair : occurrence_counts) {
        // printOutputList(count_pair.first);
        // printf("  %d\n",count_pair.second);
        if (count_pair.second > max_count) {
        max_count = count_pair.second;
        max_count_lists.clear(); // Clear previous entries with lower count
        max_count_lists.push_back(count_pair);
        } else if (count_pair.second == max_count) {
        // If another list has the same max count, add it to the list
        max_count_lists.push_back(count_pair);
        }
    }
    return max_count_lists;
}



std::string ink_decoder(double T1, double T2, double T4, double T) {
    std::vector<std::vector<std::vector<std::string>>> table = {
        {{"e6","e6","e6"}, {"o0","o0","o0"}, {"e4","e4","e4"}, {"o3","o3","o3"}},
        {{"o9","o9","o9"}, {"","e2","e8"}, {"o7","o1",""}, {"e5","e5","e5"}},
        {{"e9","e9","e9"}, {"o8","o2",""}, {"e1","e7",""}, {"o5","o5","o5"}},
        {{"o6","o6","o6"}, {"e0","e0","e0"}, {"o4","o4","o4"}, {"e3","e3","e3"}}
    };
    // printf("\nT1: %f,T2:  %f,T4:  %f\n",T1,T2,T4);

    int T1_bar = inkSpreading(T1 / T);
    int T2_bar = inkSpreading(T2 / T);
    int T4_bar = inkSpreading(T4 / T);
    
    if (T1_bar < 2) {
        T1_bar = 2;
    }
    if (T2_bar < 2) {
        T2_bar = 2;
    }
    if (T4_bar > 3) {
        T4_bar = 3;
    }
    // printf("S6\n");
    // printf("\n%d, %d, %d",T1_bar,T2_bar,T4_bar);
    // printf("\n%d, %d, %d",T1_bar - 2,T2_bar - 2,T4_bar - 1);

    return table[T1_bar - 2][T2_bar - 2][T4_bar - 1];
}

int inkSpreading(double val) {
    int mod = 0;
    
    if (val <= 1.5 / 7) {
        mod = 1;
    } else if (val >= 1.5 / 7 && val < 2.5 / 7) {
        mod = 2;
    } else if (val >= 2.5 / 7 && val < 3.5 / 7) {
        mod = 3;
    } else if (val >= 3.5 / 7 && val < 4.5 / 7) {
        mod = 4;
    } else if (val >= 4.5 / 7) {
        mod = 5;
    }

    return mod;
}

void analyse(const std::vector<int>& consecutive_counts, int i, bool dir_left, std::vector<char>& output) {
    double T1, T2, T4;

    if (dir_left) {
        T4 = consecutive_counts[i];
        T2 = consecutive_counts[i + 1] + consecutive_counts[i + 2];
        T1 = consecutive_counts[i + 2] + consecutive_counts[i + 3];
    } else {
        T1 = consecutive_counts[i] + consecutive_counts[i + 1];
        T2 = consecutive_counts[i + 1] + consecutive_counts[i + 2];
        T4 = consecutive_counts[i + 3];
    }

    double T = consecutive_counts[i] + consecutive_counts[i + 1] + consecutive_counts[i + 2] + consecutive_counts[i + 3];

    // Call ink_decoder and assign the result to the output vector
    std::string result = ink_decoder(T1, T2, T4, T);
    
    output.assign(result.begin(), result.end());
    
}

std::vector<int> analyze_consecutive_occurrences(const std::vector<int>& data) {
    std::vector<int> consecutive_counts;
    int current_value = -1;
    int current_count = 0;

    for (int value : data) {
        if (value != current_value) {
            // New value encountered, update counts
            if (current_value != -1) {
                consecutive_counts.push_back(current_count);
            }
            current_value = value;
            current_count = 1;
        } else {
            // Same value encountered, increment count
            current_count++;
        }
    }

    // Add the count for the last value
    if (current_value != -1) {
        consecutive_counts.push_back(current_count);
    }

    return consecutive_counts;
}

int isGuardBit(std::vector<int>& consecutive_counts, int i) {
    int T1 = consecutive_counts[i] + consecutive_counts[i + 1];
    int T2 = consecutive_counts[i + 1] + consecutive_counts[i + 2];
    int T4 = consecutive_counts[i + 2];
    int T = consecutive_counts[i] + consecutive_counts[i + 1] + consecutive_counts[i + 2];

    int sum = 0;
    sum += (T1 == T2);
    sum += (T1 == 2 * T4);
    sum += (T2 == 2 * T4);
    sum += (T == T1 + T4);
    sum += (T == T2 + T4);

    if (sum >= 3) {
       return 1;
    } else {
       return 0;
    }
}
#include <iterator> // for std::ostream_iterator
int left_digit(std::list<std::vector<char>>& list) {
    std::unordered_map<std::string, int> table = {
        {"oooooo", 0},
        {"ooeoee", 1},
        {"ooeeoe", 2},
        {"ooeeeo", 3},
        {"oeooee", 4},
        {"oeeooe", 5},
        {"oeeeoo", 6},
        {"oeoeoe", 7},
        {"oeoeeo", 8},
        {"oeeoeo", 9}
    };

    std::string sum = "";
    int count = 0;


    // std::cout << "List Contents:" << std::endl;
    // printOutputList(list);

    for (const std::vector<char>& vec : list) {

    // Check if the vector itself is empty (not the first element)
    if (vec.empty()) {
    } else {
        sum += vec[0]; // Assuming sum is an integer to add the first character's ASCII value
        count++;
        if (count == 6) {
        break;
        }
    }
    }

    if (table.find(sum) != table.end()) {
        return table[sum];
    } else {
        return -1;
    }
}

void printOutputList(const std::list<std::vector<char>>& output_list) {
    std::cout << "[";
    for (auto it = output_list.begin(); it != output_list.end(); ++it) {
        if (it != output_list.begin()) {
            std::cout << ",";
        }
        std::cout << "[";
        for (auto char_it = it->begin(); char_it != it->end(); ++char_it) {
            if (char_it != it->begin()) {
                std::cout << ",";
            }
            std::cout << *char_it;
        }
        std::cout << "]";
    }
    std::cout << "]" << std::endl;
}


void print_list_of_output_lists(const std::list<std::list<std::vector<char>>>& list_of_output_lists) {
  std::cout << "[";

  // Iterate over each outer list (representing a row)
  bool first_row = true;
  for (const auto& outer_list : list_of_output_lists) {
    if (!first_row) {
      std::cout << std::endl; // Add newline after the first row
    }
    first_row = false;

    std::cout << "[";

    // Iterate over each inner list (representing a decoded sequence)
    bool first_inner_list = true;
    for (const auto& inner_list : outer_list) {
      if (!first_inner_list) {
        std::cout << ","; // Separate inner lists with commas
      }
      first_inner_list = false;

      // Print each character in the inner list
      std::cout << "[";
      bool first_char = true;
      for (char c : inner_list) {
        if (!first_char) {
          std::cout << ",";
        }
        first_char = false;
        std::cout << "'" << c << "'"; // Enclose characters in single quotes
      }
      std::cout << "]";
    }

    std::cout << "]"; // Close the inner list bracket for the current row
  }

  // Remove the trailing comma (if any)
  if (!list_of_output_lists.empty()) {
    std::cout << "\b"; // Backspace to remove the last comma
  }

  std::cout << "]" << std::endl;
}
