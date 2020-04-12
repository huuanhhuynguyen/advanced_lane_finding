#include "detect.h"

namespace
{
    /// Struct represents an image bar at y position.
    struct Bar {
        cv::Mat image;
        int y;
    };

    std::vector<Bar> _split_into_bars(const cv::Mat& img, int n_bars)
    {
        std::vector<Bar> bars;
        const int n_rows_per_bar = int(float(img.rows) / float(n_bars));
        for (int i = 0; i < n_bars; ++i)
        {
            // find the start and end row of the bar
            int start = i * n_rows_per_bar;
            int end = std::min(start + n_rows_per_bar, img.rows);

            // extract the bar from the image
            Bar bar;
            bar.image = img.rowRange(start, end);
            bar.y = int((start+end) / 2);

            bars.push_back(bar);
        }
        return bars;
    }
} // unnamed namespace

void split_image_left_right(const cv::Mat& img, cv::Mat& left, cv::Mat& right)
{
    int mid = int(img.cols / 2);
    left = img.colRange(0, mid);
    right = img.colRange(mid, img.cols);
}

std::vector<cv::Point2i> detect_lane_points(const cv::Mat& bin_img)
{
    std::vector<cv::Point2i> lane_points;

    // split the binary image into horizontal bars
    const int n_bars = 10;
    std::vector<Bar> bars = _split_into_bars(bin_img, n_bars);

    struct Sum {
        int x;
        int value;
        Sum(int x_, int value_) : x{x_}, value{value_} {}
    };
    const int n_windows = 20;
    const int win_cols = int(bars[0].image.cols / n_windows);
    for (const auto& bar : bars)
    {
        std::vector<Sum> sums;
        for (int i = 0; i < n_windows; ++i)
        {
            // find start and end column of the window
            int start = i * win_cols;
            int end = std::min(start + win_cols, bar.image.cols);
            cv::Mat window = bar.image.colRange(start, end);
            // calculate sum of all pixels in window
            int sum = cv::sum(window)[0];
            // save sum and x-coordinate of the window
            int x = int((start + end) / 2);
            sums.emplace_back(x, sum);
        }

        // find window with max sum, it is the lane point
        auto max_sum = std::max_element(sums.begin(), sums.end(),
                [](const auto& lhs, const auto& rhs){ return lhs.value < rhs.value; });
        if (max_sum->value > 0)
        {
            int x = max_sum->x;
            lane_points.emplace_back(x, bar.y);
        }
    }
    return lane_points;
}