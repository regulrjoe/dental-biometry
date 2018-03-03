#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <opencv2/core.hpp>

class Histogram
{
public:
    // Get histogram of an image
    static std::vector<int> GetHistogram(cv::Mat&);

    // Get histogram of a vector of values
    static std::vector<int> GetHistogram(const std::vector<int>&);

    // Get the threshold of an histogram when the amount of brightest pixels crosses a given percentage
    static int GetThreshold(const std::vector<int>&, const float&);

private:
    // Disallow creating an instance of this object
    Histogram() {}
};

#endif // HISTOGRAM_H
