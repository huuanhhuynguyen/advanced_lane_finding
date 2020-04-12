#ifndef LANE_FINDING_ADVANCED_CPP_GENERATE_H
#define LANE_FINDING_ADVANCED_CPP_GENERATE_H

#include <vector>
#include "opencv2/core.hpp"

std::vector<cv::Point2i> generate_line_points(const cv::Size& img_size,
                                              const std::vector<float>& coeff)
{
    std::vector<cv::Point2i> line_points;
    constexpr int n_points = 20;
    float dy = float(img_size.height) / n_points;
    for (int i = 0; i < n_points; ++i)
    {
        float y = i * dy;
        float x = coeff[2] * y * y + coeff[1] * y + coeff[0];
        line_points.emplace_back(int(x), int(y));
    }
    return line_points;
}

#endif //LANE_FINDING_ADVANCED_CPP_GENERATE_H
