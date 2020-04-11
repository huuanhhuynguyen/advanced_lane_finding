#ifndef LANE_FINDING_ADVANCED_CPP_DETECT_H
#define LANE_FINDING_ADVANCED_CPP_DETECT_H

#include <vector>
#include <opencv2/highgui/highgui.hpp>

void split_image_left_right(const cv::Mat& img, cv::Mat& left, cv::Mat& right);

std::vector<cv::Point2i> detect_lane_points(const cv::Mat& bin_img);

#endif //LANE_FINDING_ADVANCED_CPP_DETECT_H
