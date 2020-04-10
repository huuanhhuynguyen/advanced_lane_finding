#ifndef LANE_FINDING_ADVANCED_CPP_BINARIZE_H
#define LANE_FINDING_ADVANCED_CPP_BINARIZE_H

#include "opencv2/imgproc.hpp"

cv::Mat binarize(const cv::Mat& image)
{
    cv::Mat bin_img;
    cv::cvtColor(image, bin_img, cv::COLOR_BGR2GRAY);
    cv::threshold(bin_img, bin_img, 127, 255, cv::THRESH_BINARY);
    return bin_img;
}


#endif //LANE_FINDING_ADVANCED_CPP_BINARIZE_H
