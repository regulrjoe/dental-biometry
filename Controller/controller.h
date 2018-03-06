#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Model/segmentation.h"
#include "Model/filters.h"
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
        input_image.copyTo(filtered_image_segmentation);
        input_image.copyTo(filtered_image_tracing);
        return true;
    }

    // Get original input image
    cv::Mat getInputImage() {
        return input_image;
    }


    //// SEGMENTATION PREPROCESSING ////
    // Get segmentation filtered image
    cv::Mat getFilteredImageSegmentation() {
        return filtered_image_segmentation;
    }

    // Reset segmentation image
    void resetImageSegmentation() {
        input_image.copyTo(filtered_image_segmentation);
    }

    // Apply Median Filter to filtered_image for segmentation
    void applyMedianSegmentation() {
        filtered_image_segmentation = Filters::Median(filtered_image_segmentation, median_kernel_size_segmentation);
    }

    // Apply Bilateral Filter to filtered_image for segmentation
    void applyBilateralSegmentation() {
        filtered_image_segmentation = Filters::Bilateral(filtered_image_segmentation, bilateral_sigma_segmentation);
    }

    // Set median kernel size for segmentation
    bool setMedianKernelSizeSegmentation(const int& k) {
        if (k < 3 || k % 2 == 0 || k > 15)
            return false;
        median_kernel_size_segmentation = k;
        return true;
    }
    // Get median kernel size for segmentation
    int getMedianKernelSizeSegmentation() {
        return median_kernel_size_segmentation;
    }
    // Set bilateral sigma for segmentation
    bool setBilateralSigmaSegmentation(const int& s) {
        if (s < 1 || s > 30)
            return false;
        bilateral_sigma_segmentation = s;
        return true;
    }
    // Get bilateral sigma for segmentation
    int getBilateralSigmaSegmentation() {
        return bilateral_sigma_segmentation;
    }


    //// SEGMENTATION ////
    bool runSegmentation() {
        if (input_image.empty())
            return false;
        filtered_image_segmentation = segmentation->Process(filtered_image_segmentation);
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
    // Set Spline curve percentge sample size of segmentation algorithm
    bool setSegmentationSplinePctSampleSize(const float& ss) {
        return segmentation->setSplinePctSampleSize(ss);
    }
    // Get Spline cruve percentage sample size of segmentation algorithm
    float getSegmentationSplinePctSampleSize() {
        return segmentation->getSplinePctSampleSize();
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


    //// TRACING PREPROCESSING ////
    // Get tracing filtered image
    cv::Mat getFilteredImageTracing() {
        return filtered_image_tracing;
    }

    // Reset tracing image
    void resetImageTracing() {
        input_image.copyTo(filtered_image_tracing);
    }

    // Apply Median Filter to tracing filtered_image
    void applyMedianTracing() {
        filtered_image_tracing = Filters::Median(filtered_image_tracing, median_kernel_size_tracing);
    }

    // Apply Bilateral Filter to tracing filtered_image
    void applyBilateralTracing() {
        filtered_image_tracing = Filters::Bilateral(filtered_image_tracing, bilateral_sigma_tracing);
    }

    // Apply Sobel Filter to tracing filtered_image
    void applySobelTracing() {
        filtered_image_tracing = Filters::Sobel(filtered_image_tracing, sobel_kernel_size_tracing, sobel_derivative_type_tracing);
    }

    // Set median kernel size for tracing
    bool setMedianKernelSizeTracing(const int& k) {
        if (k < 3 || k % 2 == 0 || k > 15)
            return false;
        median_kernel_size_tracing = k;
        return true;
    }
    // Get median kernel size for tracing
    int getMedianKernelSizeTracing() {
        return median_kernel_size_tracing;
    }
    // Set bilateral sigma for tracing
    bool setBilateralSigmaTracing(const int& s) {
        if (s < 1 || s > 30)
            return false;
        bilateral_sigma_tracing = s;
        return true;
    }
    // Get bilateral sigma for tracing
    int getBilateralSigmaTracing() {
        return bilateral_sigma_tracing;
    }
    // Set sobel kernel size for tracing
    bool setSobelKernelSizeTracing(const int& k) {
        if (k < 1 || k % 2 == 0 || k > 15)
            return false;
        sobel_kernel_size_tracing = k;
        return true;
    }
    // Get sobel kernel size for tracing
    int getSobelKernelSizeTracing() {
        return sobel_kernel_size_tracing;
    }
    // Set sobel derivative type for tracing
    bool setSobelDerivativeType(const int& t) {
        if (t < 0 || t > 2)
            return false;
        sobel_derivative_type_tracing = t;
        return true;
    }
    // Get sobel derivative type for tracing
    int getSobelDerivativeType() {
        return sobel_derivative_type_tracing;
    }

    //// TRACING ////

private:
    //// ATTRIBUTES ////
    // Pointer to singleton
    static Controller *singleton;
    // Segmentation class instance
    Segmentation *segmentation;
    // Original input image
    cv::Mat input_image;
    // Filtered image for segmentation algorithm
    cv::Mat filtered_image_segmentation;
    // Median Filter kernel size for segmentation algorithm
    int median_kernel_size_segmentation = 5;
    // Bilateral Filter sigma size/color for segmentation algorithm
    int bilateral_sigma_segmentation = 9;
    // Filtered image for tracing algorithm
    cv::Mat filtered_image_tracing;
    // Median filter kernel size for tracing algorithm
    int median_kernel_size_tracing = 3;
    // Bilateral filter sigma size/color for tracing algorithm
    int bilateral_sigma_tracing = 11;
    // Sobel filter kernel size for tracing algorithm
    int sobel_kernel_size_tracing = 3;
    // Sobel filter type for tracing algorithm
    int sobel_derivative_type_tracing = 0;

    //// METHODS ////
    // Private constructor
    Controller() {
        // Setting up the application
        segmentation = new Segmentation();
    }
};

#endif // CONTROLLER_H
