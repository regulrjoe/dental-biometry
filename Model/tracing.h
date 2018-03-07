#ifndef TRACING_H
#define TRACING_H

#include <iostream>
#include <opencv2/core.hpp>

using namespace std;

class Tracing
{
public:
    // Empty default constructor
    Tracing(): _slope_angle_distance(3),
        _first_pixel_intensity_threshold(20),
        _first_pixel_inner_margin(20),
        _crown_trace_max_pct_height(0.7),
        _crown_trace_extrapolation_distance(2),
        _crown_trace_extrapolation_mask(3) {
        cout << "Created instance of Tracing." << endl;
    }

    // Destructor
    ~Tracing() {
        cout << "Destroyed instance of Tracing." << endl;
    }

    // Run algorithm
    cv::Mat Process(const cv::Mat&);


    //// SETTERS AND GETTERS ////
    // Set slope and angle distance measurement.
    bool setSlopeAngleDistance(const int& d) {
        if (d < 1)
            return false;
        _slope_angle_distance = d;
        return true;
    }
    // Get slope and angle distance measurement.
    int getSlopeAngleDistance() {
        return _slope_angle_distance;
    }
    // Set intensity threshold for finding the first pixel of contour.
    bool setFirstPixelIntensityThreshold(const int& thr) {
        if (thr < 1 || thr > 255)
            return false;
        _first_pixel_intensity_threshold = thr;
        return true;
    }
    // Get intensity threshold for finding the first pixel of contour.
    int getFirstPixelIntensityThreshold() {
        return _first_pixel_intensity_threshold;
    }
    // Set inner margin for finding the first pixel of contour.
    bool setFirstPixelInnerMargin(const int& m) {
        if (m < 0 || m > 100)
            return false;
        _first_pixel_inner_margin = m;
        return true;
    }
    // Get inner margin for finding the first pixel of contour.
    int getFirstPixelInnerMargin() {
        return _first_pixel_inner_margin;
    }
    // Set relative max height of crown tracing
    bool setCrownTracingMaxPctHeight(const float& h) {
        if (h <= 0 || h >= 1)
            return false;
        _crown_trace_max_pct_height = h;
        return true;
    }
    // Get relative max height of crown tracing
    float getCrownTracingMaxPctHeight() {
        return _crown_trace_max_pct_height;
    }
    // Set the extrapolation distance for crown tracing
    bool setCrownTracingExtrapolationDistance(const int& d) {
        if (d < 1 || d > 100)
            return false;
        _crown_trace_extrapolation_distance = d;
        return true;
    }
    // Get the extrapolation distance for crown tracing
    int getCrownTracingExtrapolationDistance() {
        return _crown_trace_extrapolation_distance;
    }
    // Set mask of fittest pixel finding for crown tracing
    bool setCrownTracingExtrapolationMask(const int& m) {
        if (m < 3 || m % 2 == 0)
            return false;
        _crown_trace_extrapolation_mask = m;
        return true;
    }
    // Get mask of fittest pixel finding for crown tracing
    int getCrownTracingExtrapolationMask() {
        return _crown_trace_extrapolation_mask;
    }

private:
    //// INTERNAL OBJECTS ////
    // Local copy of input image for processing.
    cv::Mat _image;
    // Local copy of _image for drawing and displaying.
    cv::Mat _display_image;
    // Vector of points in resulting contour of tooth.
    vector<cv::Point> _contour;
    // Vector of slopes at each point in contour.
    vector<float> _slopes;
    // Vector of angles at each point in contour.
    vector<float> _angles;


    //// PARAMETERS ////
    // Slope and Angle distance of measurement.
    int _slope_angle_distance;
    // Intensity threshold for finding the first pixel of contour.
    int _first_pixel_intensity_threshold;
    // Inner margin of image delimiting the area where the first pixel of the contour will be searched for.
    int _first_pixel_inner_margin;
    // Max height of crown tracing in pct relative to image height
    float _crown_trace_max_pct_height;
    // Extrapolation distance for crown tracing
    int _crown_trace_extrapolation_distance;
    // Mask where fittest pixel in extrapolated neighborhood is found for crown tracing
    int _crown_trace_extrapolation_mask;

    //// METHODS ////
    // Find the first pixel from where the tracing starts
    cv::Point FindFirstContourPixel(const int&, const int&);

    // Trace the crown of the tooth down to the neck
    // Only works with lower jaw teeth
    void TraceCrown(const float&, const int&, const int&);

    // From input pixel obtain slope and angle, and append all to their respective vectors.
    void AddPixelValuesToVectors(const cv::Point&);

    // Find the brightest pixel (not already in _contour) in the neighborhood of input pixel
    cv::Point BrightestPixelInNeighborhood(const cv::Point&, const bool&, const bool&, const bool&, const bool&);

    // Get the fittest pixel inside a KxK mask.
    cv::Point FittestPixelInMask(const cv::Point&, const int&);
};

#endif // TRACING_H
