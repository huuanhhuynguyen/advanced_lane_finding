#include "calibrate.h"

void Calibrator::write_xml(const std::string &xml_file) const
{
    cv::FileStorage fs(xml_file, cv::FileStorage::WRITE);
    fs << "K" << _K << "D" << _D;
}

void Calibrator::read_xml(const std::string &xml_file)
{
    cv::FileStorage fs(xml_file, cv::FileStorage::READ);
    fs["K"] >> _K;
    fs["D"] >> _D;
}

cv::Mat Calibrator::undistort(const cv::Mat& image) const
{
    cv::Mat undistorted_img;
    cv::undistort(image, undistorted_img, _K, _D);
    return undistorted_img;
}

void ChessboardCalibrator::read_calib_images(const std::vector<cv::Mat> &images)
{
    // 2D points in image plane
    std::vector<std::vector<cv::Point2f>> img_points;
    // the same points in 3D
    std::vector<std::vector<cv::Point3f>> obj_points;

    // Init 3D chessboard points (0, 0, 0), (0, 1, 0), ..., (8, 5, 0)
    std::vector<cv::Point3f> obj_p;
    for(int y = 0; y < 6; ++y)
        for(int x = 0; x < 9; ++x)
            obj_p.emplace_back(x, y, 0);

    for (const auto& image: images)
    {
        // Convert to gray image
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // Find chessboard corners
        std::vector<cv::Point2f> corners;
        cv::Size pattern_size(9,6);
        bool found = cv::findChessboardCorners(gray, pattern_size, corners,
                                               cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
                                               + cv::CALIB_CB_FAST_CHECK);

        // If corners found, save them in image points
        if (!found) {
            std::cout << "WARN: A calibration image has no checkboard detected."
                      << std::endl;
        } else {
            // Uncomment to visualize detected chessboard corners
            // cv::Mat vis_img = image.clone();
            // cv::drawChessboardCorners(vis_img, pattern_size, corners, found);
            // display(vis_img);

            img_points.push_back(corners);
            obj_points.push_back(obj_p);
        }
    }

    cv::calibrateCamera(obj_points, img_points, images[0].size(),
                        _K, _D, {}, {});
}