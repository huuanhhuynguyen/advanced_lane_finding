#include "read.h"
#include "calibrate.h"
#include "binarize.h"
#include "birdeye.h"
#include "detect.h"
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

        // Rectify image to BEV
        const auto bev_img = bird_eye_view(bin_img);
        display(bev_img);

        // Split image into two
        cv::Mat left_img, right_img;
        split_image_left_right(bev_img, left_img, right_img);

        // Detect lane points (including outliers)
        std::vector<cv::Point2i> left_points = detect_lane_points(left_img);
        std::vector<cv::Point2i> right_points = detect_lane_points(right_img);

        // Fit points
        // Lane left_lane, right_lane;
        // fit(left_points, left_lane, right_lane);

        // Calculate curvature and vehicle offset from the lane center

        // Warp the detected lane boundaries back to original image

        // Visualize lane, boundaries, curvature, center offset

        // Display the image
        //display(rect_img);
    }

    return 0;
}
