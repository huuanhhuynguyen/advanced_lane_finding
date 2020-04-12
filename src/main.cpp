#include "read.h"
#include "calibrate.h"
#include "binarize.h"
#include "birdeye.h"
#include "detect.h"
#include "draw.h"
#include "polyfit.h"
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
    std::vector<cv::Mat> images = read_images("../data/test_images");

    // for every image/frame
    for (const auto& image : images)
    {
        // Undistort the original image
        const auto undistorted_img = calibrator.undistort(image);

        // Binarize the image
        const auto bin_img = binarize(undistorted_img);

        // Warp image to BEV
        BEV bev(bin_img);
        const auto bev_img = bev.warp(bin_img);

        // Split image into two
        cv::Mat left_img, right_img;
        split_image_left_right(bev_img, left_img, right_img);

        // Detect points of the left and right lanes on BEV
        std::vector<cv::Point2i> left_points, right_points;
        {
            // left points on the left image, which have the same coordinates
            // as on the original BEV image before splitting.
            left_points = detect_lane_points(left_img);
            // right points on the right image
            std::vector<cv::Point2i> rpoints = detect_lane_points(right_img);
            // adjust the right points to the original BEV image
            int mid = int(bev_img.cols / 2);
            auto translate_x = [mid](const cv::Point2i& point){
                return cv::Point2i(point.x+mid, point.y); };
            std::transform(rpoints.begin(), rpoints.end(),
                           std::back_inserter(right_points), translate_x);
        }

        // Uncomment to visualize lane points on BEV image
        // cv::Mat bev_img_color;
        // cv::cvtColor(bev_img, bev_img_color, cv::COLOR_GRAY2BGR);
        // draw_points(bev_img_color, left_points, cv::Scalar(255, 0, 0));
        // draw_points(bev_img_color, right_points, cv::Scalar(0, 255, 0));
        // display(bev_img_color);

        // Fit points with second polynomial order
        std::vector<float> coeff_left, coeff_right;
        {
            std::vector<float> xs, ys;
            std::transform(left_points.begin(), left_points.end(), std::back_inserter(xs), [](auto& p){ return p.x; });
            std::transform(left_points.begin(), left_points.end(), std::back_inserter(ys), [](auto& p){ return p.y; });
            coeff_left = polyfit_boost(ys, xs, 2);
        }
        {
            std::vector<float> xs, ys;
            std::transform(right_points.begin(), right_points.end(), std::back_inserter(xs), [](auto& p){ return p.x; });
            std::transform(right_points.begin(), right_points.end(), std::back_inserter(ys), [](auto& p){ return p.y; });
            coeff_right = polyfit_boost(ys, xs, 2);
        }

        // Generate line points on BEV
        left_points = generate_line_points(bev_img.size(), coeff_left);
        right_points = generate_line_points(bev_img.size(), coeff_right);

        //TODO rename variables to avoid confusion

        // Get two bottom points on the original image
        std::vector<cv::Point2i> lpoints = bev.unwarp_points(left_points);
        std::vector<cv::Point2i> rpoints = bev.unwarp_points(right_points);

        // Calculate curvature and vehicle offset from the lane center
        int vehicle_y = int(bin_img.cols / 2);
        float offset = calculate_offset_from_center(vehicle_y, lpoints[0].x, rpoints[0].x);
        float curvature = calculate_curvature(coeff_left, coeff_right);

        // Warp the detected lane boundaries back to original image

        // Visualize lane, boundaries, curvature, center offset

        // Display the image
        //display(rect_img);
    }

    return 0;
}
