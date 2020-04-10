#include "display.h"

void display(const cv::Mat& image)
{
    namedWindow( "", cv::WINDOW_AUTOSIZE );
    imshow( "", image );
    cv::waitKey(0);
}