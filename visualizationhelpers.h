#ifndef VISUALIZATIONHELPERS_H
#define VISUALIZATIONHELPERS_H

#include <vector>
#include <opencv2/core.hpp>

class VisualizationHelpers
{
public:
    // Mark an X at input points on input image
    static cv::Mat DrawXAtPoints(const cv::Mat&, const std::vector<cv::Point>&, const cv::Vec3b&, const int& = 2);

    // Draw an image-length horizontal line at input row on input image
    static cv::Mat DrawRow(const cv::Mat&, const int&, const cv::Vec3b&);

    // Draw a vector of points on input image
    static cv::Mat DrawVector(const cv::Mat&, const std::vector<cv::Point>&, const cv::Vec3b&);

private:
    // Disallow creating an instance of this object
    VisualizationHelpers() {}
};

#endif // VISUALIZATIONHELPERS_H
