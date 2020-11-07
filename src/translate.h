#ifndef LANE_FINDING_ADVANCED_CPP_TRANSLATE_H
#define LANE_FINDING_ADVANCED_CPP_TRANSLATE_H

#include <vector>
#include "opencv2/core.hpp"

std::vector<cv::Point2i>
translate(const std::vector<cv::Point2i>& points, int dx, int dy)
{
    std::vector<cv::Point2i> results;
    auto translate_fn = [dx, dy](const auto& point){ return cv::Point2i(point.x+dx, point.y+dy); };
    std::transform(points.begin(), points.end(), std::back_inserter(results), translate_fn);
    return results;
}

#endif //LANE_FINDING_ADVANCED_CPP_TRANSLATE_H
