#include "segmentation.h"
#include "filters.h"
#include "helpers.h"
#include "visualizationhelpers.h"
#include <opencv2/opencv.hpp>


// Run algorithm
cv::Mat Segmentation::Process(const cv::Mat& input) {
    cout << "Running Segmentation..." << endl;
    _image = input.clone();
    _display_image = cv::Mat::zeros(input.cols, input.rows, CV_8UC3);
    // Convert from grayscale to RGB for drawing purposes
    cv::cvtColor(input, _display_image, CV_GRAY2RGB, 3);

    // Define upper and lower crown points in image
    DefineCrownPoints(_lineprofile_column_spacing, _lineprofile_derivative_distance);
    // Remove crown points too far from avg row to be valid
    RemoveAfarCrownPoints();
    // Visualize crown points
//    _display_image = VisualizationHelpers::DrawXAtPoints(_display_image, _crowns.first, cv::Vec3b(0, 0, 255));
//    _display_image = VisualizationHelpers::DrawXAtPoints(_display_image, _crowns.second, cv::Vec3b(255, 0, 0));

    // Adjust Spline curve to crown points
    AdjustCrownsCurve(_spline_pct_sample_size);
    // Visualize crown curves
//    _display_image = VisualizationHelpers::DrawVector(_display_image, _crown_curves.first, cv::Vec3b(0, 225, 225));
//    _display_image = VisualizationHelpers::DrawVector(_display_image, _crown_curves.second, cv::Vec3b(0, 225, 225));

    // Translate crown curves to find necks curve
    AdjustNecksCurve(_neck_sd_threshold);
    // Visualize necks curves
//    _display_image = VisualizationHelpers::DrawVector(_display_image, _necks_curves.first, cv::Vec3b(225, 0, 225));
//    _display_image = VisualizationHelpers::DrawVector(_display_image, _necks_curves.second, cv::Vec3b(225, 0, 225));

    // Binarize crowns to more easily find the gaps between teeth
    BinarizeCrowns(_crown_binarization_n_segments, _crown_binarization_pct_threshold);

    // Adjust
//    ShowDisplayImage();

    return _image;
}

// Obtain vertical line profiles of image
// INPUT: img -> image from where line profiles are obtained
// INPUT: sp -> column spacing between profiles
// INPUT: dd -> Derivative distance between values
// OUTPUT: vector of pairs <column, profile>
vector< pair< int, vector<int> > > Segmentation::DerivativeLineProfiles(const cv::Mat& img, const int& sp, const int& dd) {
    int c;
    vector< pair< int, vector<int> > > profiles;   // output map <column, vector of values>

    for (c = 0; c < img.cols; c += sp) {
        profiles.push_back(
                    pair<int, vector<int> >(
                        c,
                        Helpers::DeriveVector(
                            Helpers::GrayscaleProfile(
                                img, cv::Point(c, 0), cv::Point(c, img.rows)),
                            dd)));
    }

    return profiles;
}

// Define upper and lower crown points
void Segmentation::DefineCrownPoints(const int& column_spacing, const int& derivative_difference) {
    cout << "Defining Jaw Points... " << endl;

    // Obtain derivatives of the vertical line profiles of _image
    vector< pair< int, vector<int> > > line_profiles;
    line_profiles = DerivativeLineProfiles(
                _image,
                column_spacing,
                derivative_difference);

    // Obtain minimum and maximum derivative values of each line profile
    // Minimum derivative is an upper jaw point
    // Maximum derivative is a lower jaw point
    int i,
        col,
        min_value_row,
        max_value_row;

    for (i = 0; i < (int)line_profiles.size(); i++) {
        col = line_profiles.at(i).first;
        min_value_row = Helpers::MinValueIndex(line_profiles.at(i).second, -1, -1);
        max_value_row = Helpers::MaxValueIndex(line_profiles.at(i).second, -1, -1);

        // Minimum value's row must above maximum value's row to be valid
        if (min_value_row < max_value_row ) {
            _crowns.first.push_back(cv::Point(col, min_value_row));
            _crowns.second.push_back(cv::Point(col, max_value_row));
        }
    }
}

// Remove crown points too far from avg row to be valid
void Segmentation::RemoveAfarCrownPoints() {
    int i;

    // Obtain average row upper crown points and lower crown points
    int upper_crowns_row_sum,
        lower_crowns_row_sum;

    upper_crowns_row_sum = 0;
    lower_crowns_row_sum = 0;

    for (i = 0; i < (int)_crowns.first.size(); i++)
        upper_crowns_row_sum += _crowns.first.at(i).y;
    for (i = 0; i < (int)_crowns.second.size(); i++)
        lower_crowns_row_sum += _crowns.second.at(i).y;

    int upper_crowns_avg_row,
        lower_crowns_avg_row;

    upper_crowns_avg_row = upper_crowns_row_sum / (int)_crowns.first.size();
    lower_crowns_avg_row = lower_crowns_row_sum / (int)_crowns.second.size();

    cout << "upper crowns avg row: " << upper_crowns_avg_row << endl;
    cout << "lower crowns avg row: " << lower_crowns_avg_row << endl;

    // Obtain middle row between both averages
    int middle_avg_row;

    middle_avg_row = (upper_crowns_avg_row + lower_crowns_avg_row) / 2;

    cout << "middle avg row: " << middle_avg_row << endl;

    // Draw rows for visualization
//    _display_image = VisualizationHelpers::DrawRow(_display_image, upper_crowns_avg_row, cv::Vec3b(225, 225, 0));
//    _display_image = VisualizationHelpers::DrawRow(_display_image, lower_crowns_avg_row, cv::Vec3b(225, 225, 0));
//    _display_image = VisualizationHelpers::DrawRow(_display_image, middle_avg_row, cv::Vec3b(0, 225, 225));

    // Define limits for upper crown points and lower crown points
    pair<int, int> upper_crown_limits;
    pair<int, int> lower_crown_limits;

    upper_crown_limits.first = upper_crowns_avg_row - abs(upper_crowns_avg_row - middle_avg_row);
    upper_crown_limits.second = lower_crowns_avg_row;

    lower_crown_limits.first = upper_crowns_avg_row;
    lower_crown_limits.second = lower_crowns_avg_row + abs(lower_crowns_avg_row - middle_avg_row);

    // Remove upper crown points outside the limits
    for (i = 0; i < (int)_crowns.first.size(); i++) {
        if ((_crowns.first.at(i).y < upper_crown_limits.first) ||
             (_crowns.first.at(i).y > upper_crown_limits.second)) {
            _crowns.first.erase(_crowns.first.begin() + i);
            i--;
        }
    }

    // Remove lower crow points outside the limits
    for (i = 0; i < (int)_crowns.second.size(); i++) {
        if ((_crowns.second.at(i).y < lower_crown_limits.first) ||
                (_crowns.second.at(i).y > lower_crown_limits.second)) {
            _crowns.second.erase(_crowns.second.begin() + i);
            i--;
        }
    }
}

// Adjust Spline curve to crown points
void Segmentation::AdjustCrownsCurve(const float& pct_sample_size) {
    int upper_curve_subsample_size,
        lower_curve_subsample_size;

    upper_curve_subsample_size = (int)_crowns.first.size() * pct_sample_size;
    lower_curve_subsample_size = (int)_crowns.second.size() * pct_sample_size;

    _crown_curves.first = Helpers::FitSpline(_crowns.first, 0, _image.cols, upper_curve_subsample_size);
    _crown_curves.second = Helpers::FitSpline(_crowns.second, 0, _image.cols, lower_curve_subsample_size);
}

// Translate crowns curve to find teeth's neck
void Segmentation::AdjustNecksCurve(const float& sd_thr) {

    // Get standard deviation of the derivatives of pixel values at initial position (crown curves) of upper and lower jaw
    pair<double, double> initial_stddev;

    initial_stddev.first = Helpers::DiscreteStandardDeviation(
                Helpers::DeriveVector(
                    Helpers::GrayscaleProfile(_image, _crown_curves.first)));
    initial_stddev.second = Helpers::DiscreteStandardDeviation(
                Helpers::DeriveVector(
                    Helpers::GrayscaleProfile(_image, _crown_curves.second)));

    // Translate upper curve upwards until the new standard deviation is lower than the sd_thr of the initial standard deviaiton
    int i, j;
    int max_translation =  150; // in pixels
    int ppt = 5; // pixels per translation
    pair< vector<cv::Point>, vector<cv::Point> > current_curves; // current curves at each translation
    pair<double, double> current_stddev; // standard deviaion of the derivatives of pixel values at current curves

    // Upper Jaw
    current_curves.first = _crown_curves.first;
    for (i = 0; i < max_translation; i += ppt) {

        // Translate upper curve upwards
        for (j = 0; j < (int)current_curves.first.size(); j++) {
            // Make sure curve stays in bounds
            if (current_curves.first.at(j).y - ppt >= 0)
                current_curves.first.at(j).y -= ppt;
            else
                current_curves.first.at(j).y = 0;
        }

        // Get current standard deviation
        current_stddev.first = Helpers::DiscreteStandardDeviation(
                    Helpers::DeriveVector(
                        Helpers::GrayscaleProfile(_image, current_curves.first)));

        // Finish translating if current std dev is below sd_thr
        if (current_stddev.first < initial_stddev.first * sd_thr)
            break;
    }
    _necks_curves.first = current_curves.first;

    // Lower Jaw
    current_curves.second = _crown_curves.second;
    for (i = 0; i < max_translation; i+= ppt) {

        // Translate lower curve downwards
        for (j = 0; j < (int)current_curves.second.size(); j++) {
            // Make sure curve stays in bounds
            if (current_curves.second.at(j).y + ppt < _image.rows)
                current_curves.second.at(j).y += ppt;
            else
                current_curves.second.at(j).y = _image.rows - 1;
        }

        // Get current standard deviation
        current_stddev.second = Helpers::DiscreteStandardDeviation(
                    Helpers::DeriveVector(
                        Helpers::GrayscaleProfile(_image, current_curves.second)));

        // Finish translating if current std dev is below sd_thr
        if (current_stddev.second < initial_stddev.second * sd_thr)
            break;
    }
    _necks_curves.second = current_curves.second;
}

// Binarize crowns to more easily find the gaps between teeth
void Segmentation::BinarizeCrowns(const int& n_segments, const float& pct_thr) {
    // Segment the space between the crowns curve and the necks curve in equal n_segments.
    // Binarize each segment.

    // Distance of points at each side of current iteration point to use for the slope calculation.
    float slope_half_distance = 10;

    // Length of each segment <upper jaw, lower jaw>.
    pair<int, int> segment_length;
    // The segment lenght is the length of the curve minus the full slope distance divided by n_segments.
    segment_length.first = ( (int)_crown_curves.first.size() - (slope_half_distance * 2) ) / n_segments;
    segment_length.second = ( (int)_crown_curves.second.size() - (slope_half_distance * 2) ) / n_segments;

    // Maximum height of each segment <upper jaw, lower jaw>
    pair<int, int> max_segment_height;
    // The maximum segment height is the 80% of the difference between crown and neck curves
    max_segment_height.first = abs(_crown_curves.first.at(0).y - _necks_curves.first.at(0).y) * 0.7;
    max_segment_height.second = abs(_crown_curves.second.at(0).y - _necks_curves.second.at(0).y) * 0.7;

    float       slope; // Slope at iteration point.
    bool        in_bounds; // Flag to determine if point is still inside the image.
    cv::Point   cvp; // Current point in vertical scan.
    cv::Point   upper_polygon[4]; // Points of the polygon of each upper jaw segment.
    cv::Point   lower_polygon[4]; // Points of the polygon of each lower jaw segment.
    int         n, i, j;

    for (n = 1; n < n_segments; n++) {
        // Upper Jaw
        i = (n * segment_length.first) + slope_half_distance; // element position of current segment in crowns curve
        in_bounds = true;
        slope = Helpers::GetSlope(
                    _crown_curves.first.at(i - slope_half_distance),
                    _crown_curves.first.at(i + slope_half_distance));
        // Iterate upwards
        j = 0;
        do {
            j++;
            cvp = cv::Point(
                        _crown_curves.first.at(i).x + (j * slope),
                        _crown_curves.first.at(i).y - j);
            // Make sure vertical scan stays in bounds
            if (cvp.x < 5 || cvp.x > _image.cols - 5 || cvp.y < 5)
                in_bounds = false;
        } while (j < max_segment_height.first && in_bounds);

        if (n > 1) {
            upper_polygon[0] = upper_polygon[1];
            upper_polygon[3] = upper_polygon[2];
        }
        // Move point in the crowns curve 30 pixels inwards to better capture the crown
        upper_polygon[1] = cv::Point(_crown_curves.first.at(i).x, _crown_curves.first.at(i).y + 30);
        upper_polygon[2] = cvp;

        if (n > 1)
            _image = Filters::PolygonBinarization(_image, upper_polygon, 4, pct_thr, _image);

        // Lower Jaw
        i = (n * segment_length.second) + slope_half_distance; // element position of current segment in crowns curve
        in_bounds = true;
        slope = Helpers::GetSlope(
                    _crown_curves.second.at(i - slope_half_distance),
                    _crown_curves.second.at(i + slope_half_distance));
        // Iterate downwards
        j = 0;
        do {
            j++;
            cvp = cv::Point(
                        _crown_curves.second.at(i).x - (j * slope),
                        _crown_curves.second.at(i).y + j);
            // Make sure vertical scan stays in bounds
            if (cvp.x < 5 || cvp.x > _image.cols - 5 || cvp.y > _image.rows - 5)
                in_bounds = false;
        } while(j < max_segment_height.first && in_bounds);

        if (n > 1) {
            lower_polygon[0] = lower_polygon[1];
            lower_polygon[3] = lower_polygon[2];
        }

        // Move point in the crowns curve 30 pixels inwards to better capture the crown
        lower_polygon[1] = cv::Point(_crown_curves.second.at(i).x, _crown_curves.second.at(i).y - 30);
        lower_polygon[2] = cvp;

        if (n > 1)
            _image = Filters::PolygonBinarization(_image, lower_polygon, 4, pct_thr, _image);
    }
}

//// HELPFUL VISUALIZATION METHODS ////

// Display draw image
void Segmentation::ShowDisplayImage() {
    cv::imshow("Display Image", _display_image);
}

