#include "read.h"
#include "calibrate.h"
#include "binarize.h"
#include "birdeye.h"
#include "detect.h"
#include "translate.h"
#include "draw.h"
#include "curve.h"
#include "polyfit.h"
#include "smoothen.h"
#include "generate.h"
#include "offset.h"
#include "curvature.h"
#include "display.h"

int main()
{
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
    //std::vector<cv::Mat> images = read_images("../data/test_images");
    std::vector<cv::Mat> images = read_video_frames("../data/project_video.mp4");

    // Moving average
    CurveMoveAvg left_mov_avg{10}, right_mov_avg{10};

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

        // Uncomment to visualize lane points on BEV image
        // cv::Mat bev_img_color = bev_img.clone();
        // draw_points(bev_img_color, lpoints_bev, cv::Scalar(0, 0, 255));
        // draw_points(bev_img_color, rpoints_bev, cv::Scalar(255, 0, 0));
        // display(bev_img_color);

        // Fit points with second polynomial order
        SecondPolynomial left_polyn, right_polyn;
        {
            std::vector<float> xs, ys;
            std::transform(lpoints_bev.begin(), lpoints_bev.end(), std::back_inserter(xs), [](auto& p){ return p.x; });
            std::transform(lpoints_bev.begin(), lpoints_bev.end(), std::back_inserter(ys), [](auto& p){ return p.y; });
            std::vector<float> coeffs = polyfit_boost(ys, xs, 2);
            left_polyn = SecondPolynomial(coeffs[2], coeffs[1], coeffs[0]);
        }
        {
            std::vector<float> xs, ys;
            std::transform(rpoints_bev.begin(), rpoints_bev.end(), std::back_inserter(xs), [](auto& p){ return p.x; });
            std::transform(rpoints_bev.begin(), rpoints_bev.end(), std::back_inserter(ys), [](auto& p){ return p.y; });
            std::vector<float> coeffs = polyfit_boost(ys, xs, 2);
            right_polyn = SecondPolynomial(coeffs[2], coeffs[1], coeffs[0]);
        }

        // Smoothen
        left_polyn = left_mov_avg.update(left_polyn);
        right_polyn = right_mov_avg.update(right_polyn);

        // Generate line points on BEV
        lpoints_bev = generate_line_points(bev_img.size(), left_polyn);
        rpoints_bev = generate_line_points(bev_img.size(), right_polyn);

        // Unwarp bev points to original image
        std::vector<cv::Point2i> lpoints = bev.unwarp_points(lpoints_bev);
        std::vector<cv::Point2i> rpoints = bev.unwarp_points(rpoints_bev);

        // Calculate curvature and vehicle offset from the lane center
        float offset, curvature;
        {
            int ego_x = bin_img.cols / 2;
            int bottom_mid_x = (lpoints[0].x + rpoints[0].x) / 2;
            offset = calculate_offset_in_meter(ego_x, bottom_mid_x);
            curvature = calculate_curvature(left_polyn, right_polyn);
        }

        //TODO make naming of curve, 2ndpolynomial, etc. consistent

        // Visualize lane
        auto vis_image = image.clone();
        std::vector<cv::Point2i> lane_points(lpoints.size());
        std::reverse_copy(lpoints.begin(), lpoints.end(), lane_points.begin());
        lane_points.insert(lane_points.end(), rpoints.begin(), rpoints.end());
        draw_polygon(vis_image, lane_points);

        // Draw lane boundaries
        draw_curve(vis_image, lpoints, cv::Scalar(0, 0, 255));
        draw_curve(vis_image, rpoints, cv::Scalar(255, 0, 0));

        // Visualize curvature and offset

        // Display the image
        display(vis_image);
    }

    return 0;
}
