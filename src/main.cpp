#include <iomanip>

#include "read.h"
#include "calibrate.h"
#include "binarize.h"
#include "birdeye.h"
#include "detect.h"
#include "translate.h"
#include "draw.h"
#include "coefficient.h"
#include "polyfit.h"
#include "smoothen.h"
#include "generate.h"
#include "offset.h"
#include "radius.h"
#include "display.h"

enum Mode : uint8_t {
    IMAGE, VIDEO
};

int main()
{
    Mode mode = VIDEO;

    // Create an image calibrator from
    //     - chessboard images (if not done the calibrator before), OR
    //     - a yaml file, where the calibration values have been saved
    ChessboardCalibrator calibrator;
    {
        // Uncomment the following if calibrate with chessboard images again
        // std::vector<cv::Mat> calib_images = read_images("../data/camera_calib");
        // calibrator.read_calib_images(calib_images);
        // calibrator.write_xml("../data/calib.xml");
        calibrator.read_xml("../data/calib.xml");
    }

    // Read images / video
    std::vector<cv::Mat> images;
    if (mode == IMAGE) {
        images = read_images("../data/test_images");
    } else {
        images = read_video_frames("../data/project_video.mp4");
    }

    // Moving average
    CoeffMoveAvg left_mov_avg{10}, right_mov_avg{10};

    // for every image/frame
    for (const auto& image : images)
    {
        // Undistort the original image
        const auto undistorted_img = calibrator.undistort(image);

        // Binarize the image
        const auto bin_img = binarize(undistorted_img);

        // Warp image to BEV
        BEVWarper bev(bin_img.size());
        const auto bev_img = bev.warp(bin_img);

        // Detect lane points on BEV
        std::vector<cv::Point2i> lpoints_bev, rpoints_bev;
        {
            // Split image into two
            cv::Mat left_img, right_img;
            split_image_left_right(bev_img, left_img, right_img);

            // detect points on each half image
            lpoints_bev = detect_lane_points(left_img);

            std::vector<cv::Point2i> rpoints = detect_lane_points(right_img);
            // adjust the right points to the original BEV
            int mid = int(bev_img.cols / 2);
            rpoints_bev = translate(rpoints, mid, 0);
        }

        // Fit points with second polynomial order
        Coefficient left_coeff, right_coeff;
        {
            std::vector<float> xs, ys;
            std::transform(lpoints_bev.begin(), lpoints_bev.end(), std::back_inserter(xs), [](auto& p){ return p.x; });
            std::transform(lpoints_bev.begin(), lpoints_bev.end(), std::back_inserter(ys), [](auto& p){ return p.y; });
            std::vector<float> coeffs = polyfit_boost(ys, xs, 2);
            left_coeff = Coefficient(coeffs[2], coeffs[1], coeffs[0]);
        }
        {
            std::vector<float> xs, ys;
            std::transform(rpoints_bev.begin(), rpoints_bev.end(), std::back_inserter(xs), [](auto& p){ return p.x; });
            std::transform(rpoints_bev.begin(), rpoints_bev.end(), std::back_inserter(ys), [](auto& p){ return p.y; });
            std::vector<float> coeffs = polyfit_boost(ys, xs, 2);
            right_coeff = Coefficient(coeffs[2], coeffs[1], coeffs[0]);
        }

        // Smoothen
        if (mode == VIDEO)
        {
            left_coeff = left_mov_avg.update(left_coeff);
            right_coeff = right_mov_avg.update(right_coeff);
        }

        // Generate line points on BEV
        lpoints_bev = generate_line_points(bev_img.size(), left_coeff);
        rpoints_bev = generate_line_points(bev_img.size(), right_coeff);

        // Unwarp bev points to original image
        std::vector<cv::Point2i> lpoints = bev.unwarp_points(lpoints_bev);
        std::vector<cv::Point2i> rpoints = bev.unwarp_points(rpoints_bev);

        // Calculate curve radius and vehicle offset from the lane center
        float offset, radius;
        {
            int ego_x = bin_img.cols / 2;
            int bottom_mid_x = (lpoints[0].x + rpoints[0].x) / 2;
            offset = calculate_offset_in_meter(ego_x, bottom_mid_x);
            float radius_left = calculate_curve_radius(left_coeff);
            float radius_right = calculate_curve_radius(right_coeff);
            radius = (radius_left + radius_right) / 2;
        }

        // Visualize lane
        auto vis_image = undistorted_img.clone();
        std::vector<cv::Point2i> lane_points(lpoints.size());
        std::reverse_copy(lpoints.begin(), lpoints.end(), lane_points.begin());
        lane_points.insert(lane_points.end(), rpoints.begin(), rpoints.end());
        draw_polygon(vis_image, lane_points);

        // Draw lane boundaries
        draw_curve(vis_image, lpoints, cv::Scalar(0, 0, 255));
        draw_curve(vis_image, rpoints, cv::Scalar(255, 0, 0));

        // Visualize binary image
        {
            cv::Mat small_img;
            cv::resize(bin_img, small_img, bin_img.size() / 5);
            overlay(small_img, vis_image, 10, 10);
        }

        // Visualize bev image
        {
            cv::Mat small_img;
            cv::resize(bev_img, small_img, bev_img.size() / 5);
            overlay(small_img, vis_image, 10*2 + small_img.cols, 10);
        }

        // Visualize bev image with detected points
        {
            cv::Mat bev_img_color = bev_img.clone();
            draw_points(bev_img_color, lpoints_bev, cv::Scalar(0, 0, 255));
            draw_points(bev_img_color, rpoints_bev, cv::Scalar(255, 0, 0));
            cv::Mat small_img;
            cv::resize(bev_img_color, small_img, bev_img_color.size() / 5);
            overlay(small_img, vis_image, 10*3 + small_img.cols*2, 10);
        }

        // Visualize offset
        {
            std::ostringstream stream;
            stream << "Center offset = " << std::fixed << std::setprecision(3) << offset << " m";
            std::string text = stream.str();
            cv::putText(vis_image, text, cv::Point2i(900, 30),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8,
                        cv::Scalar(220,220,250));
        }

        // Visualize curve radius
        {
            std::ostringstream stream;
            stream << "Curve radius = " <<  std::fixed << std::setprecision(1) << radius << " m";
            std::string text = stream.str();
            cv::putText(vis_image, text, cv::Point2i(900, 60),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8,
                        cv::Scalar(220,220,250));
        }

        // Display the image
        display(vis_image);
    }

    return 0;
}
