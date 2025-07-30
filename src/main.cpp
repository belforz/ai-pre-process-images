#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string path = "bw-11.jpg";
    cv::Mat image = cv::imread(path);

    if (image.empty()) {
        std::cerr << " Error: could not load " << path << std::endl;
        return 1;
    }

    std::cout << " Image loaded successfully!" << std::endl;
    std::cout << " Dimensions: " << image.cols << " x " << image.rows << std::endl;

    return 0;
}
