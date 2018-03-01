#include <iostream>
#include <opencv2/imgproc.hpp>
#include "filters.h"
#include "histogram.h"


// Apply median filter on input image
cv::Mat Filters::Median(const cv::Mat& input, const int& kernel_size) {
    std::cout << "Applying median filter..." << std::endl;
    cv::Mat output;

    input.copyTo(output);

    cv::blur(input, output, cv::Size(kernel_size, kernel_size));

    return output;
}

// Apply bilateral filter on input image
cv::Mat Filters::Bilateral(const cv::Mat& input, const int& sigmas) {
    std::cout << "Applying bilateral filer..." << std::endl;
    cv::Mat output;

    input.copyTo(output);

    cv::bilateralFilter(input, output, 0, sigmas, sigmas);

    return output;
}

// Apply contrast enhancement on image with top-hat and bottom-hat transforms
cv::Mat Filters::ContrastEnhancement(const cv::Mat& input, const float& struct_width, const float& struct_height, const int& struct_type) {
    std::cout << "Applying contrast enhancement..." << std::endl;
    cv::Mat output;

    input.copyTo(output);

    output = output + TopHat(input, struct_width, struct_height, struct_type) - BottomHat(input, struct_width, struct_height, struct_type);

    return output;
}

// Apply Top-hat transform on input image
cv::Mat Filters::TopHat(const cv::Mat& input, const float& struct_width, const float& struct_height, const int& struct_type) {
    std::cout << "Applying Top-Hat transform..." << std::endl;
    cv::Mat output, element;
    cv::Size struct_size;
    // Create structure size relative to input image
    struct_size.width = input.cols * struct_width;
    struct_size.height = input.rows * struct_height;
    // Create structure element
    element = cv::getStructuringElement(struct_type, struct_size);
    // Apply opening operation
    cv::morphologyEx(input, output, cv::MORPH_TOPHAT, element);

    return output;
}

// Apply Bottom-hat transform on input image
cv::Mat Filters::BottomHat(const cv::Mat& input, const float& struct_width, const float& struct_height, const int& struct_type) {
    std::cout << "Applying Bottom-Hat transform..." << std::endl;
    cv::Mat output, element;
    cv::Size struct_size;
    // Create structure size relative to input image
    struct_size.width = input.cols * struct_width;
    struct_size.height = input.rows * struct_height;
    // Create structure element
    element = cv::getStructuringElement(struct_type, struct_size);
    // Apply closing operation
    cv::morphologyEx(input, output, cv::MORPH_BLACKHAT, element);

    return output;
}

// Apply Closing operation (Erosion -> Dilation)
cv::Mat Filters::Closing(const cv::Mat& input, const int& struct_width, const int& struct_height, const int& struct_type) {
    std::cout << "Applying closing operation..." << std::endl;
    cv::Mat output, element;
    // Create structure element
    element = cv::getStructuringElement(struct_type, cv::Size(struct_width, struct_height));
    // Apply erosion operation
    cv::morphologyEx(input, output, 2, element);

    return output;
}

// Apply Erosion transform to input image
cv::Mat Filters::Erode(const cv::Mat& input, const int& struct_width, const int& struct_height, const int& struct_type) {
    std::cout << "Applying erosion..." << std::endl;
    cv::Mat output, element;
    // Create structure element
    element = cv::getStructuringElement(struct_type, cv::Size(struct_width, struct_height));
    // Apply erosion operation
    cv::erode(input, output, element);

    return output;
}

// Apply binarization to input image
cv::Mat Filters::Binarization(const cv::Mat& input, const float& thr) {
    std::cout << "Applying binarization..." << std::endl;
    cv::Mat output;
    // Apply binarization
    cv::threshold(input, output, thr, 255, 0);

    return output;
}

// Apply binarization to polygon of input image
// This implementation receives input image and polynomial of input image and applies binarization to it
//  Input:
//      input = Input image
//      points = polynomial points [topleft, topright, bottomright, bottomleft]
//          points must be ordered clockwise
//      pct_thr = Percentage threshold of binarization
cv::Mat Filters::PolygonBinarization(const cv::Mat& input, const cv::Point* pts, const int& npts, const float& pct_thr, const cv::Mat output) {
    std::cout << "Applying binarization..." << std::endl;
    cv::Mat             local_output;
    cv::Mat             mask;
    cv::Mat             masked;
    cv::Point           topleft, botright;
    std::vector<int>    values;
    std::vector<int>    histogram;

    int i, j, thr;

    if (output.empty())
        input.copyTo(local_output);
    else
        output.copyTo(local_output);

    if (local_output.size != input.size) {
        std::cout << "Input and output image must be of equal size." << std::endl;
        return cv::Mat();
    }

    // Find size of polygon in rows and cols
    topleft = botright = pts[0];
    for (i = 1; i < npts; i++) {
        if (pts[i].x < topleft.x)
            topleft.x = pts[i].x;
        else if (pts[i].x > botright.x)
            botright.x = pts[i].x;

        if (pts[i].y < topleft.y)
            topleft.y = pts[i].y;
        else if (pts[i].y > botright.y)
            botright.y = pts[i].y;
    }

    // Create mask image
    mask = cv::Mat::zeros(input.rows, input.cols, CV_8U);
    // Paint polygon white on mask image
    cv::fillConvexPoly(mask, pts, npts, 255);


    // Store value of pixels inside polygon
    for (i = topleft.x; i <= botright.x; i++)
        for (j = topleft.y; j <= botright.y; j++)
            if (mask.at<uchar>(cv::Point(i,j)) == 255)
                values.push_back(input.at<uchar>(cv::Point(i, j)));

    // Get histogram from values
    histogram = Histogram::GetHistogram(values);
    // Get static threshold from histogram and relative threshold
    thr = Histogram::GetThreshold(histogram, pct_thr);
    std::cout << "Static threshold: " << thr << std::endl;

    // Apply polygon mask on input image
    input.copyTo(masked, mask);
    // Binarize masked image with static threhsold
    masked = Binarization(masked, thr);
    // Copy binarized area to output image
    masked.copyTo(local_output, mask);

    return local_output;
}

// Apply local binarization to input image
cv::Mat Filters::LocalBinarization(const cv::Mat& input, float pct_thr, const int& n_rows, const int& n_cols) {
    std::cout << "Applying local binarization..." << std::endl;
    cv::Mat     output;
    cv::Mat     local;
    cv::Size    local_size;
    cv::Point   ij_point;
    int i, j, thr;

    output = cv::Mat::zeros(input.rows, input.cols, CV_8U);
    local_size = cv::Size(input.cols / n_cols, input.rows / n_rows);

    //pct_thr is inversely proportional to the amount of subregions
    pct_thr = pct_thr + (pct_thr * log(n_rows * n_cols));

    for (i = 0; i < n_rows; i++) {
        for (j = 0; j < n_cols; j++) {
            // Obtian current point of iteration
            ij_point = cv::Point(local_size.width * j, local_size.height * i);
            // Create local subimage according to current region
            local = input(cv::Rect(ij_point, local_size));
            // Obtain int threshold of local subimage
            thr = Histogram::GetThreshold(Histogram::GetHistogram(local), pct_thr);
            // Binarize local subimage
            local = Binarization(local, thr);
            // Append local binarized subimage to output
            local.copyTo(output(cv::Rect(ij_point, local_size)));
        }
    }
    return output;
}
