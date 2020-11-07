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
    cv::Mat mask_H = (hsv[0] <= 30);
    cv::Mat mask_S = (hsv[1] >= 100);
    cv::Mat mask_V = (hsv[2] >= 100);
    cv::Mat mask = mask_H & mask_S & mask_V;
    mask.convertTo(mask, image.depth());

    return mask;
}

cv::Mat gray_white_mask(const cv::Mat& image)
{
    // Gray image
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Equalize histogram (important step improves the result significantly)
    cv::Mat equalized_gray;
    cv::equalizeHist(gray, equalized_gray);

    // Mask for the white color
    cv::Mat mask;
    cv::threshold(equalized_gray, mask, /*thresh=*/252,
                  /*maxval=*/255, cv::THRESH_BINARY);

    return mask;
}

cv::Mat binarize(const cv::Mat& image)
{
    auto bin_img = cv::Mat(image.size(), image.depth(), double(0));

    // highlight yellow color by thresholding in HSV color space
    {
        const auto mask = hsv_yellow_mask(image);
        bin_img = bin_img | mask;
        // display(mask);
    }

    // highlight white lines by thresholding the equalized frame
    {
        const auto mask = gray_white_mask(image);
        bin_img = bin_img | mask;
        // display(mask);
    }

    // apply (rectangular closing) morphological transformation
    {
        const auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::morphologyEx(bin_img, bin_img, cv::MORPH_CLOSE, kernel);
        // display(bin_img);
    }

    return bin_img;
}

#endif //LANE_FINDING_ADVANCED_CPP_BINARIZE_H
