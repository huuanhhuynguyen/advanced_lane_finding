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

void draw_polygon(cv::Mat& img, const std::vector<cv::Point2i>& points)
{
    const int n_points = points.size();
    cv::Mat poly_img = img.clone();

    // Based on source: https://docs.opencv.org/3.2.0/d3/d96/tutorial_basic_geometric_drawing.html
    cv::Point2i rook_points[1][n_points];
    for (int i = 0; i < points.size(); ++i)
    {
        rook_points[0][i] = points[i];
    }
    const cv::Point2i* polygons[1] = { rook_points[0] };

    int npt[] = { n_points };
    cv::fillPoly(poly_img, polygons, npt, /*ncontours=*/1, cv::Scalar( 0, 255, 0));

    // blend image
    cv::addWeighted(poly_img, /*alpha=*/0.4, img, /*beta=*/0.6, /*gamma=*/0.0, img);
}

void draw_curve(cv::Mat& img, const std::vector<cv::Point2i>& points,
                const cv::Scalar& color)
{
    if (points.size() < 2) {
        throw std::invalid_argument("There must be at least 2 points to draw.");
    }

    for (int i = 1; i < points.size(); ++i) {
        cv::line(img, points[i - 1], points[i], color, /*thickness=*/5);
    }
}

#endif //LANE_FINDING_ADVANCED_CPP_DRAW_H
