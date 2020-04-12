#ifndef LANE_FINDING_ADVANCED_CPP_DRAW_H
#define LANE_FINDING_ADVANCED_CPP_DRAW_H

#include <opencv2/imgproc.hpp>

void draw_points(cv::Mat& img, const std::vector<cv::Point2i>& points,
                 const cv::Scalar& color)
{
    if (img.channels() == 1) {
        cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    }

    for (const auto& point : points) {
        cv::circle(img, point, /*radius=*/10, color, /*thickness=*/-1);
    }
}

#endif //LANE_FINDING_ADVANCED_CPP_DRAW_H
