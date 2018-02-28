#ifndef FILTERING_H
#define FILTERING_H

#include <opencv2/core.hpp>

class Filtering
{
    // Disallow creating an instance of this object
    Filtering() {}

public:
    // Apply contrast enhancement on image with top-hat and bottom-hat transforms
    static cv::Mat ContrastEnhancement(const cv::Mat&, const float&, const float&, const int& = 0);
    // Apply Top-hat transform on input image
    static cv::Mat TopHat(const cv::Mat&, const float&, const float&, const int& = 0);
    // Apply Bottom-hat transform on input image
    static cv::Mat BottomHat(const cv::Mat&, const float&, const float&, const int& = 0);
    // Apply Closing operation (Erosion -> Dilation)
    static cv::Mat Closing(const cv::Mat&, const int&, const int&, const int& = 0);
    // Apply Erosion transform to input image
    static cv::Mat Erode(const cv::Mat&, const int&, const int&, const int& = 0);
    // Apply binarization to input image
    static cv::Mat Binarization(const cv::Mat&, const float&);
    // Apply binarization to polygon in input image
    static cv::Mat PolygonBinarization(const cv::Mat&, const cv::Point*, const int&, const float&, const cv::Mat = cv::Mat());
    // Apply local binarization to input image
    static cv::Mat LocalBinarization(const cv::Mat&, float, const int&, const int&);
};

#endif // FILTERING_H
