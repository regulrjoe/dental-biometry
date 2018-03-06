#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

using namespace std;

class Segmentation
{
public:
    // Empty default constructor
    Segmentation() : _lineprofile_column_spacing(5),
        _lineprofile_derivative_distance(5),
        _spline_pct_sample_size(0.2),
        _neck_sd_threshold(0.45),
        _crown_binarization_n_segments(30),
        _crown_binarization_pct_threshold(0.25) {
        cout << "Created instance of Segmentation." << endl;
    }

    // Destructor
    ~Segmentation() {
        cout << "Destroyed instance of Segmentation." << endl;
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
    // Set Spline curve percentage sample size
    bool setSplinePctSampleSize(const float& ss) {
        if (ss <= 0 || ss > 1)
            return false;
        _spline_pct_sample_size = ss;
        return true;
    }
    // Get Spline curve percentage sample size
    float getSplinePctSampleSize() {
        return _spline_pct_sample_size;
    }
    // Set necks curves standard deviation threhsold
    bool setNecksCurvesStdDevThreshold(const float& thr) {
        if (thr <= 0 || thr >= 1)
            return false;
        _neck_sd_threshold = thr;
        return true;
    }
    // Get necks curves standard deviation threhsold
    float getNecksCurvesStdDevThreshold() {
        return _neck_sd_threshold;
    }
    // Set crowns binarization number of segments
    bool setCrownBinarizationNumOfSegments(const int& n) {
        if (n < 1 || n > 100)
            return false;
        _crown_binarization_n_segments = n;
        return true;
    }
    // Get crowns binarization number of segments
    int getCrownBinarizationNumOfSegments() {
        return _crown_binarization_n_segments;
    }
    // Set crowns binarization percentage threshold
    bool setCrownBinarizationPctThreshold(const float& thr) {
        if (thr <= 0 || thr >= 1)
            return false;
        _crown_binarization_pct_threshold = thr;
        return true;
    }
    // Get crowns binarization percentage threshold
    float getCrownBinarizationPctThreshold() {
        return _crown_binarization_pct_threshold;
    }

private:
    //// INTERNAL OBJECTS ////
    // Local copy of input image for processing
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
    float _spline_pct_sample_size;
    // Std Dev threshold for finding the necks curve
    float _neck_sd_threshold;
    // Number of segments for binarization of crowns
    int _crown_binarization_n_segments;
    // Percanetage threhsold for binariation of crowns
    float _crown_binarization_pct_threshold;

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

    // Binarize crowns to more easily find the gaps between teeth
    void BinarizeCrowns(const int&, const float&);

    // Show display image
    void ShowDisplayImage();


};

#endif // SEGMENTATION_H
