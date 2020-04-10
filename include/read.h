#ifndef BASIC_LANE_FINDING_CPP_READ_H
#define BASIC_LANE_FINDING_CPP_READ_H

#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

/// list all files in a directory
/// source: www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
std::vector<std::string> list_dir(const std::string& directory)
{
    std::vector<std::string> result;

    boost::filesystem::path p(directory);
    boost::filesystem::directory_iterator start(p);
    boost::filesystem::directory_iterator end;

    auto path_leaf_string = [](const auto& entry){ return entry.path().string(); };
    std::transform(start, end, std::back_inserter(result), path_leaf_string);

    return result;
}

/// Read all files in a directory and return a list of images
std::vector<cv::Mat> read_images(const std::string& directory)
{
    auto filenames = list_dir(directory);

    std::vector<cv::Mat> images;
    auto img_read = [](const auto& path){ return cv::imread(path); };
    std::transform(filenames.begin(), filenames.end(), std::back_inserter(images), img_read);

    return images;
}

/// Read all frames of a video file and save in a list of images.
/// Assumption: the video is small-sized. Otherwise, a frame generator should be
/// implemented instead.
std::vector<cv::Mat> read_video_frames(const std::string& path)
{
    cv::VideoCapture capture(path);

    // Check if camera opened successfully
    if (!capture.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return {};
    }

    // Read each video frame and save in a list
    std::vector<cv::Mat> frames;
    while (true)
    {
        cv::Mat frame;
        capture.read(frame);
        if (frame.empty()) {
            break;
        }

        frames.push_back(frame);
    }

    return frames;
}

#endif //BASIC_LANE_FINDING_CPP_READ_H
