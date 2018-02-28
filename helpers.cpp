#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core.hpp>
#include "helpers.h"
#include "spline.h"


// Get the slope of two pixels
double Helpers::GetSlope(const cv::Point &p1, const cv::Point &p2) {
    double dy, dx;

    dx = p2.x - p1.x;
    dy = p2.y - p1.y;

    // If dx != 0, return dy/dx, and 0 otherwise
    return (dx != 0) ? (dy/dx) : (0);
}

// Get the angle of two pixels
double Helpers::GetAngle(const cv::Point &p1, const cv::Point &p2, const bool &degrees) {
    double dy, dx, angle;

    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    angle = atan2(dy, dx);

    if (degrees)
        angle *= 180/3.141592653589;

    return angle;
}

// Extrapolate a coordinate linearly from the angle
cv::Point Helpers::LinearExtrapolation(const cv::Point &anchor, const double &angle, const int &distance) {
    cv::Point ex;
    int dx, dy;
    //Find x difference and y difference
    dx = round(distance * cos(angle));
    dy = round(distance * sin(angle));
    //Add difference to point p
    ex = cv::Point(anchor.x + dx, anchor.y + dy);
    //Return currentpixel +x,y difference
    return ex;
}

// Interpolation with Newton method
// Input: a = x coordinate to interpolate
//        x = X coordinates
//        y = Y coordinates
//        n = distance
int Helpers::NewtonInterpolation(const int &a, std::vector<double> x, std::vector<double> y, const int &n) {
    int j = 0,
        i = 0,
        sum_int;

    double  sum = 0,
            mult;

    for (j; j < n-1; j++)
        for (i = n-1; i > j; i--)
            y[i] = (y[i] - y[i-1]) / (x[i] - x[i-j-1]);

    for (i = n-1; i >= 0; i--) {
        mult = 1;

        for (j = 0; j < i; j++)
            mult *= (a - x[j]);

        mult *= y[j];
        sum += mult;
    }

    sum_int = (int)round(sum);

    return sum_int;
}

// Get current time as string with input format
std::string Helpers::GetCurrentDateTimeAsString() {
    std::cout << "Getting current datetime" << std::endl;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[300];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", timeinfo);
    std::string str(buffer);

    return buffer;
}

// Get standard deviation of a vector of discrete values
double Helpers::DiscreteStandardDeviation(const std::vector<int> &v) {
    double mean, std_dev;
    int n, sum_of_distance, i;

    n = v.size();
    mean = 0;
    sum_of_distance = 0;

    // Find mean of population
    for (i = 0; i < n; i++)
        mean += v.at(i);
    mean /= n;

    // For each data point, find the square distance to the mean and sum the values
    for (i = 0; i < n; i++)
        sum_of_distance += pow(fabs(v.at(i) - mean), 2);

    // Divide the sum by the number of data points and square root it
    std_dev = sqrt(double(sum_of_distance) / double(n));

    return std_dev;

}

// Derive the values of a vector.
// INPUT: v -> Vector to derive.
// INPUT: d -> distance between values to derive.
// OUTPUT: vector of derivatives
std::vector<int> Helpers::DeriveVector(const std::vector<int>& v, const int& d) {
    std::vector<int> derivatives;
    int i;
    derivatives.push_back(0);
    for (i = 1; i < (int)v.size(); i++) {
        if (i > d)
            derivatives.push_back(v.at(i) - v.at(i-d));
        else
            derivatives.push_back(v.at(i) - v.at(0));
    }

    return derivatives;
}

// Get the max value of a vector of integers at a given range
int Helpers::MaxValueIndex(const std::vector<int> &v, int min_limit, int max_limit) {
    int index;

    if (min_limit == -1)
        min_limit = 0;
    if (max_limit == -1)
        max_limit = v.size()-1;

    auto max_it = std::max_element(v.begin()+min_limit, v.begin()+max_limit);

    index = max_it - v.begin();

    return index;
}

// Get the min value of a vector of integers at a given range
int Helpers::MinValueIndex(const std::vector<int> &v, int min_limit, int max_limit) {
    int index;

    if (min_limit == -1)
        min_limit = 0;
    if (max_limit == -1)
        max_limit = v.size()-1;

    auto min_it = std::min_element(v.begin()+min_limit, v.begin()+max_limit);

    index = min_it - v.begin();

    return index;
}

// Get the grayscale profile of a line between two points
std::vector<int> Helpers::GrayscaleProfile(const cv::Mat &img, cv::Point p1, cv::Point p2) {
    std::vector<int> profile;
    cv::Point buffer;
    double slope;
    double angle;
    int i;

    slope = GetSlope(p1, p2);
    angle = GetAngle(p1, p2);

    std::cout << "Slope: " << slope << std::endl;
    std::cout << "Angle: " << angle << std::endl;

    if (fabs(angle) < 45 || fabs(angle) > 135) { // Move through x
        std::cout << "Moving through X" << std::endl;

        if (p1.x > p2.x) {
            buffer = p1;
            p1 = p2;
            p2 = buffer;
        }

        for (i = 0; i <= p2.x - p1.x; i++)
            profile.push_back(img.at<uchar>(cv::Point(p1.x + i, p1.y + (slope*i))));
    } else { // Move through y
        std::cout << "Moving through Y" << std::endl;

        if (p1.y > p2.y) {
            buffer = p1;
            p1 = p2;
            p2 = buffer;
        }

        if (slope == 0) // Vertical line. Avoid dividing by 0
            for (i = 0; i <= p2.y - p1.y; i++)
                profile.push_back(img.at<uchar>(cv::Point(p1.x, p1.y + i)));
        else // Diagonal
            for (i = 0; i <= p2.y - p1.y; i++)
                profile.push_back(img.at<uchar>(cv::Point(p1.x + (i/slope), p1.y + i)));
    }

    return profile;
}

// Get the grayscale profile of a vector of points
std::vector<int> Helpers::GrayscaleProfile(const cv::Mat &img, const std::vector<cv::Point> &p) {
    std::vector<int> profile;
    int i;

    for (i = 0; i < (int)p.size(); i++)
        profile.push_back(img.at<uchar>(p.at(i)));

    return profile;
}

// Fit a Spline function line to a group of jaw points
std::vector<cv::Point> Helpers::FitSpline(const std::vector<cv::Point>& v, const int& min_x, const int& max_x, const int& subsamples) {
    // Loop iterator
    int i;
    // Size of input Vector
    int n;
    // Distance between samples
    int d;
    // Ouput vector of points
    std::vector<cv::Point> curve;
    // Input vector of x coordinates
    std::vector<double> X;
    // Input vector of y coordinates
    std::vector<double> Y;

    n = v.size();

    if (subsamples == -1)
        d = 1;
    else
        d = n / subsamples;

    // Use only the subsamples of X and Y coordinates
    for (i = 0; i < n; i += d) {
        X.push_back((double)v.at(i).x);
        Y.push_back((double)v.at(i).y);
    }
    // Add last X and Y coordinates to vectors
    if (X.back() != v.back().x && Y.back() != v.back().y) {
        X.push_back((double)v.back().x);
        Y.push_back((double)v.back().y);
    }

    // Use TK Spline Class
    tk::spline spline;
    spline.set_points(X, Y);

    for (i = min_x; i < max_x; i++)
        curve.push_back(cv::Point(i, spline(i)));

    return curve;
}
