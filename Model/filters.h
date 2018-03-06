#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/core.hpp>

class Filters
{
public:
    // Apply median filter on input image
    static cv::Mat Median(const cv::Mat&, const int&);

    // Apply bilateral filter on input image
    static cv::Mat Bilateral(const cv::Mat&, const int&);

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

    // Apply sobel filter to input image
    static cv::Mat Sobel(const cv::Mat&, const int& = 3, const int& = 0);

private:
    // Disallow creating an instance of this object
    Filters() {}
};

#endif // FILTERS_H
