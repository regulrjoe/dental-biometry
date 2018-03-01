#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vector>
#include <opencv2/core.hpp>

using namespace std;

class Segmentation
{
public:
    // Default constructor
    Segmentation();

    // Parameterized constructor
//    Segmentation();

    // Destructor
    ~Segmentation();

    // Run algorithm
    cv::Mat Process(const cv::Mat&);


    //// SETTERS AND GETTERS ////
    // Get line profiles column spacing
    int getLineProfileColumnSpacing() {
        return _lineprofile_column_spacing;
    }
    // Set line profiles column spacing
    bool setLineProfileColumnSumnpacing(const int& cs) {
        if (cs < 1 || cs > 100)
            return false;
        _lineprofile_column_spacing = cs;
        return true;
    }
    // Get line profiles derivative distance
    int getLineProfileDerivativeDistance() {
        return _lineprofile_derivative_distance;
    }
    // Set line profiles derivative distance
    bool setLineProfileDerivativeDistance(const int& dd) {
        if (dd < 1 || dd > 100)
            return false;
        _lineprofile_derivative_distance = dd;
        return true;
    }

private:
    //// ATTRIBUTES ////
    // Local copy of input image
    cv::Mat _image;
    // Local copy of _image for drawing and displaying
    cv::Mat _display_image;
    // Column spacing between line profiles
    int _lineprofile_column_spacing;
    // Distance between values in line profile to derive
    int _lineprofile_derivative_distance;
    // Pair of vectors with crown points <upper crowns, lower crowns>
    pair< vector<cv::Point>, vector<cv::Point> > _crowns;


    //// METHODS ////
    // Define upper and lower crown points
    void DefineCrownPoints();

    // Obtain derivatives of the vertical line profiles of image
    vector< pair< int, vector<int> > > DerivativeLineProfiles(const cv::Mat&, const int&, const int&);


    //// HELPFUL VISUALIZATION METHODS ////
    // Mark an X at input point
    cv::Mat DrawXAtPoints(const cv::Mat&, const vector<cv::Point>&, const cv::Vec3b&, const int& = 2);

    // Show display image
    void ShowDisplayImage();


};

#endif // SEGMENTATION_H
