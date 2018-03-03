#include "visualizationhelpers.h"
#include <opencv2/opencv.hpp>


// Mark an X at input point
cv::Mat VisualizationHelpers::DrawXAtPoints(const cv::Mat& input, const std::vector<cv::Point>& points, const cv::Vec3b& color, const int& x_size) {
    int i, j;
    cv::Mat output;

    input.copyTo(output);
    if (output.channels() < 3)
        cv::cvtColor(output, output, CV_GRAY2RGB, 3);

    for (i = 0; i < (int)points.size(); i++) {
        output.at<cv::Vec3b>(points.at(i)) = color;
        for (j = 1; j <= x_size; j++) {
            output.at<cv::Vec3b>(cv::Point(points.at(i).x-j, points.at(i).y-j)) = color;
            output.at<cv::Vec3b>(cv::Point(points.at(i).x+j, points.at(i).y-j)) = color;
            output.at<cv::Vec3b>(cv::Point(points.at(i).x-j, points.at(i).y+j)) = color;
            output.at<cv::Vec3b>(cv::Point(points.at(i).x+j, points.at(i).y+j)) = color;
        }
    }

    return output;
}

// Draw an image-length horizontal line at input row
cv::Mat VisualizationHelpers::DrawRow(const cv::Mat& input, const int& row, const cv::Vec3b& color) {
    cv::Mat output;

    input.copyTo(output);
    if (output.channels() < 3)
        cv::cvtColor(output, output, CV_GRAY2RGB, 3);

    cv::line(output,
             cv::Point(0, row),
             cv::Point(output.cols-1, row),
             color);

    return output;
}

// Draw a vector of points on input image
cv::Mat VisualizationHelpers::DrawVector(const cv::Mat& input, const std::vector<cv::Point>& points, const cv::Vec3b& color) {
    int i;
    cv::Mat output;

    input.copyTo(output);
    if (output.channels() < 3)
        cv::cvtColor(output, output, CV_GRAY2RGB, 3);

    for (i = 0; i < (int)points.size(); i++)
        output.at<cv::Vec3b>(points.at(i)) = color;

    return output;
}
