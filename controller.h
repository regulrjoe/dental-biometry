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
        input_image = cv::imread(filename);
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

    void restoreImage() {
        input_image.copyTo(filtered_image);
    }

    //// PREPROCESSING ////
    bool applyMedian(const int& kernel_size) {
        if (kernel_size < 3 || k % 2 == 0)
            return false;
        filtered_image = Filters::Median(filtered_image, kernel_size);
        return true;
    }

    bool applyBilateral(const int& sigmas) {
        if (sigmas < 0)
            return false;
        filtered_image = Filters::Bilateral(filtered_image, sigmas);
        return true;
    }

    //// SEGMENTATION ////
    bool runSegmentation() {
        if (input_image.empty())
            return false;
        filtered_image = segmentation->Process(filtered_image);
        return true;
    }

    // Get line profiles column spacing of segmentation algorithm
    int getSegmentationLineProfileColumnSpacing() {
        return segmentation-getLineProfileColumnSpacing();
    }
    // Set line profiles column spacing of segmentation algorithm
    bool setSegmentationLineProfileColumnSpacing(const int& cs) {
        return segmentation->setLineProfileColumnSpacing(cs);
    }
    // Get line profiles derivative distance of segmentation algorithm
    int getSegmentationLineProfileDerivativeDistance() {
        return segmentation->getLineProfileDerivativeDistance();
    }
    // Set line profiles derivative distance of segmentation algorithm
    bool setSegmentationLineProfileDerivativeDistance(const int& dd) {
        return segmentation->setLineProfileDerivativeDistance(dd);
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


    //// METHODS ////
    // Private constructor
    Controller() {
        // Setting up the application
        segmentation = new Segmentation();
    }
};

#endif // CONTROLLER_H
