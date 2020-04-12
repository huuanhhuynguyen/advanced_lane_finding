#ifndef LANE_FINDING_ADVANCED_CPP_BIRDEYE_H
#define LANE_FINDING_ADVANCED_CPP_BIRDEYE_H

#include "opencv2/imgproc.hpp"

class BEV {
public:
    explicit BEV(const cv::Mat& image)
    {
        h = float(image.rows);
        w = float(image.cols);
        _src = {
                {w, h-10}, {0, h-10}, {546, 460}, {732, 460}
        };
        _dst = {
                {w, h}, {0, h}, {0, 0}, {w, 0}
        };
        M = cv::getPerspectiveTransform(_src, _dst);
        uM = cv::getPerspectiveTransform(_dst, _src);
    }

    cv::Mat warp(const cv::Mat& image) const
    {
        cv::Mat warped;
        cv::warpPerspective(image, warped, M, image.size());
        return warped;
    }

    cv::Mat unwarp(const cv::Mat& image) const
    {

        cv::Mat unwarped;
        cv::warpPerspective(image, unwarped, uM, image.size());
        return unwarped;
    }

    std::vector<cv::Point2i> unwarp_points(const std::vector<cv::Point2i>& points)
    {
        std::vector<cv::Point2f> points_f;
        std::copy(points.begin(), points.end(), std::back_inserter(points_f));

        std::vector<cv::Point2f> unwarped_points;
        cv::perspectiveTransform(points_f, unwarped_points, uM);

        std::vector<cv::Point2i> unwarped_points_i;
        std::copy(unwarped_points.begin(), unwarped_points.end(), std::back_inserter(unwarped_points_i));
        return unwarped_points_i;
    }

private:
    std::vector<cv::Point2f> _src, _dst;
    float h, w;
    cv::Mat M;  // perspective matrix
    cv::Mat uM; // unwarping perspective matrix
};

#endif //LANE_FINDING_ADVANCED_CPP_BIRDEYE_H
