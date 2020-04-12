#ifndef LANE_FINDING_ADVANCED_CPP_GENERATE_H
#define LANE_FINDING_ADVANCED_CPP_GENERATE_H

#include <vector>
#include "opencv2/core.hpp"
#include "coefficient.h"

std::vector<cv::Point2i> generate_line_points(const cv::Size& img_size,
                                              const Coefficient& coeff,
                                              int n_points=40)
{
    std::vector<cv::Point2i> line_points;
    float dy = float(img_size.height) / float(n_points);
    for (int i = 0; i < n_points; ++i)
    {
        float y = float(i) * dy;
        float x = coeff.a * y * y + coeff.b * y + coeff.c;
        line_points.emplace_back(int(x), int(y));
    }
    return line_points;
}

#endif //LANE_FINDING_ADVANCED_CPP_GENERATE_H
