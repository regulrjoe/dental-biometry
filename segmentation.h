#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

using namespace std;

class Segmentation
{
public:
    // Empty constructor
    Segmentation() : _lineprofile_column_spacing(5), _lineprofile_derivative_distance(5), _spline_relative_sample_size(0.2), _neck_sd_threshold(0.45) {
        cout << "Create instance of Segmentation." << endl;
    }

    // Destructor
    ~Segmentation() {
        cout << "Destroying instance of Segmentation." << endl;
    }

    // Run algorithm
    cv::Mat Process(const cv::Mat&);


    //// SETTERS AND GETTERS ////
    // Set line profiles column spacing
    bool setLineProfileColumnSpacing(const int& cs) {
        if (cs < 1 || cs > 100)
            return false;
        _lineprofile_column_spacing = cs;
        return true;
    }
    // Get line profiles column spacing
    int getLineProfileColumnSpacing() {
        return _lineprofile_column_spacing;
    }
    // Set line profiles derivative distance
    bool setLineProfileDerivativeDistance(const int& dd) {
        if (dd < 1 || dd > 100)
            return false;
        _lineprofile_derivative_distance = dd;
        return true;
    }
    // Get line profiles derivative distance
    int getLineProfileDerivativeDistance() {
        return _lineprofile_derivative_distance;
    }
    // Set Spline curve relative sample size
    bool setSplineRelativeSampleSize(const float& ss) {
        if (ss <= 0 || ss > 1)
            return false;
        _spline_relative_sample_size = ss;
        return true;
    }
    // Get Spline curve relative sample size
    float getSplineRelativeSampleSize() {
        return _spline_relative_sample_size;
    }
    // Set necks curves standard deviation threhsold
    bool setNecksCurvesStdDevThreshold(const float& thr) {
        if (thr <= 0 || thr >= 1)
            return false;
        _neck_sd_threshold = thr;
        return true;
    }
    // Get necks curves standard deviation threhsold
    float getNecksCurvesStdDevThrehsold() {
        return _neck_sd_threshold;
    }

private:
    //// INTERNAL OBJECTS ////
    // Local copy of input image
    cv::Mat _image;
    // Local copy of _image for drawing and displaying
    cv::Mat _display_image;
    // Pair of vectors with crown points <upper crowns, lower crowns>
    pair< vector<cv::Point>, vector<cv::Point> > _crowns;
    // Pair of vectors with crown curve points <upper crowns curve, lower crowns curve>
    pair< vector<cv::Point>, vector<cv::Point> > _crown_curves;
    // Pair of vectors with neck curve points <upper necks curve, lower necks curve>
    pair< vector<cv::Point>, vector<cv::Point> > _necks_curves;

    //// PARAMETERS ////
    // Column spacing between line profiles
    int _lineprofile_column_spacing;
    // Distance between values in line profile to derive
    int _lineprofile_derivative_distance;
    // Sample size of crown points for adjusting Spline curve
    float _spline_relative_sample_size;
    // Std Dev threshold for finding the necks curve
    float _neck_sd_threshold;

    //// METHODS ////
    // Obtain derivatives of the vertical line profiles of image
    vector <pair < int, vector<int> > > DerivativeLineProfiles(const cv::Mat&, const int&, const int&);

    // Define upper and lower crown points
    void DefineCrownPoints(const int&, const int&);

    // Remove crown points too far from avg row to be valid
    void RemoveAfarCrownPoints();

    // Adjust Spline curve to crown points
    void AdjustCrownsCurve(const float&);

    // Translate crowns curve to find teeth's neck
    void AdjustNecksCurve(const float&);

    // Show display image
    void ShowDisplayImage();


};

#endif // SEGMENTATION_H
