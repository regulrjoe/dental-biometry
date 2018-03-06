#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "segmentation.h"
#include "filters.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class Controller
{
public:
    // Get access to Singleton instance
    static Controller *getInstance() {
        // Creates the instance at first call
        if (singleton == 0)
            singleton = new Controller;
        return singleton;
    }

    // Release singleton instance of this controller
    static void destroy() {
        if (singleton != 0) {
            delete singleton;
            singleton = 0;
        }
    }

    // Delete processor objects created by controller
    ~Controller() {
        delete segmentation;
    }

    // Read image from file and set as input image
    bool setInputImage(const std::string& filename) {
        input_image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        if (!input_image.data)
            return false;
        input_image.copyTo(filtered_image);
        return true;
    }

    // Get original input image
    cv::Mat getInputImage() {
        return input_image;
    }

    // Get filtered image
    cv::Mat getFilteredImage() {
        return filtered_image;
    }

    void resetImage() {
        input_image.copyTo(filtered_image);
    }

    //// PREPROCESSING ////
    // Apply Median Filter to filtered_image
    void applyMedian() {
        filtered_image = Filters::Median(filtered_image, median_kernel_size);
    }

    // Apply Bilateral Filter to filtered_image
    void applyBilateral() {
        filtered_image = Filters::Bilateral(filtered_image, bilateral_sigma);
    }

    // Set median kernel size
    bool setMedianKernelSize(const int& k) {
        if (k < 3 || k % 2 == 0 || k > 15)
            return false;
        median_kernel_size = k;
        return true;
    }
    // Get median kernel size
    int getMedianKernelSize() {
        return median_kernel_size;
    }
    // Set bilateral sigma
    bool setBilateralSigma(const int& s) {
        if (s < 1 || s > 30)
            return false;
        bilateral_sigma = s;
        return true;
    }
    // Get bilateral sigma
    int getBilateralSigma() {
        return bilateral_sigma;
    }

    //// SEGMENTATION ////
    bool runSegmentation() {
        if (input_image.empty())
            return false;
        filtered_image = segmentation->Process(filtered_image);
        return true;
    }

    // Set line profiles column spacing of segmentation algorithm
    bool setSegmentationLineProfileColumnSpacing(const int& cs) {
        return segmentation->setLineProfileColumnSpacing(cs);
    }
    // Get line profiles column spacing of segmentation algorithm
    int getSegmentationLineProfileColumnSpacing() {
        return segmentation->getLineProfileColumnSpacing();
    }
    // Set line profiles derivative distance of segmentation algorithm
    bool setSegmentationLineProfileDerivativeDistance(const int& dd) {
        return segmentation->setLineProfileDerivativeDistance(dd);
    }
    // Get line profiles derivative distance of segmentation algorithm
    int getSegmentationLineProfileDerivativeDistance() {
        return segmentation->getLineProfileDerivativeDistance();
    }
    // Set Spline curve relative sample size of segmentation algorithm
    bool setSegmentationSplineRelativeSampleSize(const float& ss) {
        return segmentation->setSplineRelativeSampleSize(ss);
    }
    // Get Spline cruve relative sample size of segmentation algorithm
    float getSegmentationSplineRelativeSampleSize() {
        return segmentation->getSplineRelativeSampleSize();
    }
    // Set necks curves standard deviation threshold of segmentation algorithm
    bool setSegmentationNecksCurvesStdDevThreshold(const float& thr) {
        return segmentation->setNecksCurvesStdDevThreshold(thr);
    }
    // Get necks curves standard deviation threshold of segmentation algorithm
    float getSegmentationNecksCurvesStdDevThreshold() {
        return segmentation->getNecksCurvesStdDevThreshold();
    }
    // Set crown binarization number of segments of segmentation algorithm
    bool setSegmentationCrownBinarizationNumOfSegments(const int& n) {
        return segmentation->setCrownBinarizationNumOfSegments(n);
    }
    // Get crown binarization number of segments of segmentation algorithm
    int getSegmentationCrownBinarizationNumOfSegments() {
        return segmentation->getCrownBinarizationNumOfSegments();
    }
    // Set crown binarization percentage threshold of segmentation algorithm
    bool setSegmentationCrownBinarizationPctThreshold(const float& thr) {
        return segmentation->setCrownBinarizationPctThreshold(thr);
    }
    // Get crown binarization percentage threshold of segmentation algorithm
    float getSegmentationCrownBinarizationPctThreshold() {
        return segmentation->getCrownBinarizationPctThreshold();
    }

private:
    //// ATTRIBUTES ////
    // Pointer to singleton
    static Controller *singleton;
    // Segmentation class instance
    Segmentation *segmentation;
    // Original input image
    cv::Mat input_image;
    // Filtered image
    cv::Mat filtered_image;
    // Median Filter kernel size
    int median_kernel_size = 5;
    // Bilateral Filter sigma size/color
    int bilateral_sigma = 9;

    //// METHODS ////
    // Private constructor
    Controller() {
        // Setting up the application
        segmentation = new Segmentation();
    }
};

#endif // CONTROLLER_H
