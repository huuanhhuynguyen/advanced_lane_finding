#ifndef LANE_FINDING_ADVANCED_CPP_BIRDEYE_H
#define LANE_FINDING_ADVANCED_CPP_BIRDEYE_H

#include "opencv2/imgproc.hpp"

class BEVWarper {
public:
    explicit BEVWarper(const cv::Size& sz) :
        img_size{sz},
        src { {float(sz.width), float(sz.height)-10},
              {0, float(sz.height)-10},
              {546, 460},
              {732, 460} },
        dst { {float(sz.width), float(sz.height)},
              {0, float(sz.height)},
              {0, 0},
              {float(sz.width), 0} },
        M_warp   { cv::getPerspectiveTransform(src, dst) },
        M_unwarp { cv::getPerspectiveTransform(dst, src) }
    {}

    cv::Mat warp(const cv::Mat& image) const
    {
        cv::Mat warped;
        cv::warpPerspective(image, warped, M_warp, img_size);
        return warped;
    }

    std::vector<cv::Point2i> unwarp_points(const std::vector<cv::Point2i>& points)
    {
        std::vector<cv::Point2f> points_f;
        std::copy(points.begin(), points.end(), std::back_inserter(points_f));

        std::vector<cv::Point2f> unwarped_points;
        cv::perspectiveTransform(points_f, unwarped_points, M_unwarp);

        std::vector<cv::Point2i> unwarped_points_i;
        std::copy(unwarped_points.begin(), unwarped_points.end(), std::back_inserter(unwarped_points_i));
        return unwarped_points_i;
    }

private:
    cv::Size img_size;
    std::vector<cv::Point2f> src, dst;

    // perspective matrices
    cv::Mat M_warp;
    cv::Mat M_unwarp;
};

#endif //LANE_FINDING_ADVANCED_CPP_BIRDEYE_H
