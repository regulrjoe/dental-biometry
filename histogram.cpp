#include <iostream>
#include "histogram.h"

// Get histogram of an image
std::vector<int> Histogram::GetHistogram(cv::Mat& input) {
    std::cout << "Obtaining histogram..." << std::endl;

    std::vector<int> hist;
    cv::Mat_<uchar>::iterator it = input.begin<uchar>();
    int i;

    for (i = 0; i < 256; i++)
        hist.push_back(0);

    for (it; it != input.end<uchar>(); ++it)
        hist[*it]++;

    return hist;
}

// Get histogram of vector of values
std::vector<int> Histogram::GetHistogram(const std::vector<int>& values) {
    std::cout << "Obtaining histogram..." << std::endl;

    std::vector<int> hist;
    int i;

    for (i = 0; i < 256; i++)
        hist.push_back(0);

    for (i = 0; i < (int)values.size(); i++)
        hist[values.at(i)]++;

    return hist;
}

// Get static threshold of an histogram when the amount of brightest pixels crosses a given percentage
int Histogram::GetThreshold(const std::vector<int>& hist, const float& pct) {
    std::cout << "Obtaining histogram threshold at " << pct << "%%..." << std::endl;

    int i,
        sum,
        pct_pix,
        total_pix = 0;

    // Get the total pixels
    for (i = 0; i < (int)hist.size(); i++)
        total_pix += hist[i];

    // Get the pct in amount of pixels
    pct_pix = total_pix * pct;

    // Add the amount of brightest pixels until the meet the pct_pix
    i--;
    for (sum = 0; sum < pct_pix; i--)
        sum += hist[i];

    // i = threhsold
    return i;
}
