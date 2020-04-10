#ifndef LANE_FINDING_ADVANCED_CPP_BIRDEYE_H
#define LANE_FINDING_ADVANCED_CPP_BIRDEYE_H

#include "opencv2/imgproc.hpp"

cv::Mat bird_eye_view(const cv::Mat& image)
{
    const auto h = float(image.rows);
    const auto w = float(image.cols);

    std::vector<cv::Point2f> src {
        {w, h-10}, {0, h-10}, {546, 460}, {732, 460}
    };
    std::vector<cv::Point2f> dst {
        {w, h}, {0, h}, {0, 0}, {w, 0}
    };

    // perspective matrix
    cv::Mat M = cv::getPerspectiveTransform(src, dst);

    cv::Mat warped;
    cv::warpPerspective(image, warped, M, image.size());
    return warped;
}

#endif //LANE_FINDING_ADVANCED_CPP_BIRDEYE_H
