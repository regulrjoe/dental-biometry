#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <opencv2/core.hpp>

class Helpers
{
public:
    // Get the slope of two pixels
    static double GetSlope(const cv::Point&, const cv::Point&);

    // Get the angle of two pixels
    static double GetAngle(const cv::Point&, const cv::Point&, const bool& = true);

    // Check if a vector contains a point
    static bool HasPoint(const cv::Point&, const std::vector<cv::Point>&);

    // Extrapolate a coordinate linearly from the angle
    static cv::Point LinearExtrapolation(const cv::Point&, const double&, const int&);

    // Interpolation with Newton method
    static int NewtonInterpolation(const int&, std::vector<double>, std::vector<double>, const int&);

    // Get current time as string with input format
    static std::string GetCurrentDateTimeAsString();

    // Get standard deviation of a vector of discrete values
    static double DiscreteStandardDeviation(const std::vector<int>&);

    // Get derivatives vector of an input vector of values
    static std::vector<int> DeriveVector(const std::vector<int>&, const int& = 1);

    // Get the max value of a vector of integers at a given range
    static int MaxValueIndex(const std::vector<int>&, int = -1, int = -1);

    // Get the min value of a vector of integers at a given range
    static int MinValueIndex(const std::vector<int>&, int = -1, int = -1);

    // Get the grayscale profile of a line between two points
    static std::vector<int> GrayscaleProfile(const cv::Mat&, cv::Point, cv::Point);

    // Get the grayscale profile of a vector of points
    static std::vector<int> GrayscaleProfile(const cv::Mat&, const std::vector<cv::Point>&);

    // Fit a Spline function line to a group of points
    static std::vector<cv::Point> FitSpline(const std::vector<cv::Point>&, const int&, const int&, const int& = -1);

    // Get the sum of the pixel's value in a current pixel's neighborhood
    static int SumOfNeighbors(const cv::Mat&, const cv::Point&, const int&);

private:
    // Disallow creating an instance of this object
    Helpers() {}
};

#endif // HELPERS_H
