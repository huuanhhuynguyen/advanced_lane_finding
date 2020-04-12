#ifndef LANE_FINDING_ADVANCED_CPP_GENERATE_H
#define LANE_FINDING_ADVANCED_CPP_GENERATE_H

#include <vector>
#include "opencv2/core.hpp"
#include "curve.h"

std::vector<cv::Point2i> generate_line_points(const cv::Size& img_size,
                                              const SecondPolynomial& curve,
                                              int n_points=40)
{
    std::vector<cv::Point2i> line_points;
    float dy = float(img_size.height) / float(n_points);
    for (int i = 0; i < n_points; ++i)
    {
        float y = float(i) * dy;
        float x = curve.a * y * y + curve.b * y + curve.c;
        line_points.emplace_back(int(x), int(y));
    }
    return line_points;
}

#endif //LANE_FINDING_ADVANCED_CPP_GENERATE_H
