#include "detect.h"

namespace {

struct ImgBar {
    cv::Mat img_data;
    int y;
    int cols() const { return img_data.cols; }
};

std::vector<ImgBar> _split_into_bars(const cv::Mat& img, int n_bars=10)
{
    std::vector<ImgBar> bars;
    int n_rows_per_bar = int(float(img.rows) / float(n_bars));
    for (int i = 0; i < n_bars; ++i)
    {
        // find the start and end row of the bar
        int start = i * n_rows_per_bar;
        int end = std::min(start + n_rows_per_bar, img.rows);

        // extract the bar from the image
        ImgBar bar;
        bar.img_data = img.rowRange(start, end);
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
    std::vector<ImgBar> bars = _split_into_bars(bin_img);

    struct Sum {
        int x;
        int value;
        Sum(int x_, int value_) : x{x_}, value{value_} {}
    };
    int n_windows = 10;
    int win_size = int(bars[0].cols() / n_windows);
    for (const auto& bar : bars)
    {
        std::vector<Sum> sums;
        for (int i = 0; i < n_windows; ++i)
        {
            int start = i * win_size;
            int end = std::min(start + win_size, bar.cols());
            cv::Mat window = bar.img_data.colRange(start, end);
            int sum = cv::sum(window)[0];
            int mid = int((start + end) / 2);
            sums.emplace_back(mid, sum);
        }
        auto max_sum = std::max_element(sums.begin(), sums.end(),
                                        [](const auto& lhs, const auto& rhs){ return lhs.value < rhs.value; });
        if (max_sum->value > 0)
        {
            int x = max_sum->x;
            lane_points.emplace_back(x, bar.y);
        }
    }

    cv::Mat vis_img;
    cv::cvtColor(bin_img, vis_img, cv::COLOR_GRAY2BGR);
    for (const auto& point : lane_points)
    {
        cv::circle(vis_img, point, 30, cv::Scalar(0, 0, 255), -1);
    }
    display(vis_img);
    return lane_points;
}