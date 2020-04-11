#ifndef LANE_FINDING_ADVANCED_CPP_DRAW_H
#define LANE_FINDING_ADVANCED_CPP_DRAW_H

#include <opencv2/imgproc.hpp>

void draw_points(cv::Mat& img, const std::vector<cv::Point2i>& points,
                 const cv::Scalar& color)
{
    for (const auto& point : points)
    {
        cv::circle(img, point, 15, color, -1);
    }
}

#endif //LANE_FINDING_ADVANCED_CPP_DRAW_H
