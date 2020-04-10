#ifndef LANE_FINDING_ADVANCED_CPP_BINARIZE_H
#define LANE_FINDING_ADVANCED_CPP_BINARIZE_H

#include "opencv2/imgproc.hpp"
#include "display.h"

cv::Mat hsv_yellow_mask(const cv::Mat& image)
{
    // HSV image
    cv::Mat hsv_img;
    cv::cvtColor(image, hsv_img, cv::COLOR_BGR2HSV);

    // Three channels of the HSV image
    cv::Mat hsv[3];
    cv::split(hsv_img, hsv);

    // Mask for the yellow color
    cv::Mat mask_H = (hsv[0] > 0) & (hsv[0] < 50);
    cv::Mat mask_S = (hsv[1] > 70) & (hsv[1] < 255);
    cv::Mat mask_V = (hsv[2] > 70) & (hsv[2] < 255);
    cv::Mat mask = mask_H & mask_S & mask_V;
    mask.convertTo(mask, image.depth());

    // display(mask);
    return mask;
}

cv::Mat binarize(const cv::Mat& image)
{
    auto bin_img = cv::Mat(image.size(), image.depth(), double(0));

    // highlight yellow color by threshold in HSV color space
    {
        const auto mask = hsv_yellow_mask(image);
        cv::bitwise_or(bin_img, mask, bin_img);
    }

    return bin_img;
}

#endif //LANE_FINDING_ADVANCED_CPP_BINARIZE_H
