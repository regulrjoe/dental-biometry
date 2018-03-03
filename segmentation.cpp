#include "segmentation.h"
#include "helpers.h"
#include <opencv2/opencv.hpp>

// Run algorithm
cv::Mat Segmentation::Process(const cv::Mat &input) {
    cout << "Running Segmentation..." << endl;
    _image = input.clone();
    _display_image = cv::Mat::zeros(input.cols, input.rows, CV_8UC3);
    // Convert from grayscale to RGB for drawing purposes
    cv::cvtColor(input, _display_image, CV_GRAY2RGB, 3);

    // Define upper and lower crown points in image
    DefineCrownPoints();

    // Remove crown points too far from avg row to be valid
    RemoveAfarCrownPoints();

    _display_image = DrawXAtPoints(_display_image, _crowns.first, cv::Vec3b(0, 0, 255));
    _display_image = DrawXAtPoints(_display_image, _crowns.second, cv::Vec3b(255, 0, 0));

    ShowDisplayImage();

    return _image;
}

// Obtain vertical line profiles of image
// INPUT: img -> image from where line profiles are obtained
// INPUT: sp -> column spacing between profiles
// INPUT: dd -> Derivative distance between values
// OUTPUT: vector of pairs <column, profile>
vector <pair <int, vector<int> > > Segmentation::DerivativeLineProfiles(const cv::Mat& img, const int& sp, const int& dd) {
    int c;
    vector <pair <int, vector<int> > > profiles;   // output map <column, vector of values>

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
void Segmentation::DefineCrownPoints() {
    cout << "Defining Jaw Points... " << endl;

    // Obtain derivatives of the vertical line profiles of _image
    vector <pair <int, vector<int> > > line_profiles;
    line_profiles = DerivativeLineProfiles(
                _image,
                _lineprofile_column_spacing,
                _lineprofile_derivative_distance);

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

    // Obtain middle row between both averages
    int middle_avg_row;

    middle_avg_row = (upper_crowns_avg_row + lower_crowns_avg_row) / 2;

    // Draw rows for visualization
//    _display_image = DrawRow(_display_image, upper_crowns_avg_row, cv::Vec3b(225, 225, 0));
//    _display_image = DrawRow(_display_image, lower_crowns_avg_row, cv::Vec3b(225, 225, 0));
//    _display_image = DrawRow(_display_image, middle_avg_row, cv::Vec3b(0, 225, 225));

    // Define limits for upper crown points and lower crown points
    pair<int, int> upper_crown_limits;
    pair<int, int> lower_crown_limits;

    upper_crown_limits.first = upper_crowns_avg_row - abs(upper_crowns_avg_row - middle_avg_row);
    upper_crown_limits.second = middle_avg_row;

    lower_crown_limits.first = middle_avg_row;
    lower_crown_limits.second = lower_crowns_avg_row + abs(lower_crowns_avg_row - middle_avg_row);

    // Remove upper crown points outside the limits
    for (i = 0; i < (int)_crowns.first.size(); i++) {
        if (_crowns.first.at(i).y < upper_crown_limits.first ||
                _crowns.first.at(i).y > upper_crown_limits.second) {
            _crowns.first.erase(_crowns.first.begin() + i);
            i--;
        }
    }

    // Remove lower crow points outside the limits
    for (i = 0; i < (int)_crowns.second.size(); i++) {
        if (_crowns.second.at(i).y < lower_crown_limits.first ||
                _crowns.second.at(i).y > lower_crown_limits.second) {
            _crowns.second.erase(_crowns.second.begin() + i);
            i--;
        }
    }
}

//// HELPFUL VISUALIZATION METHODS ////

// Mark an X at input point
cv::Mat Segmentation::DrawXAtPoints(const cv::Mat& input, const vector<cv::Point>& points, const cv::Vec3b& color, const int& x_size) {
    int i, j;
    cv::Mat output;

    input.copyTo(output);
    if (output.channels() < 3)
        cv::cvtColor(output, output, CV_GRAY2RGB, 3);

    for (i = 0; i < (int)points.size(); i++) {
        output.at<cv::Vec3b>(points.at(i)) = color;
        for (j = 1; j <= x_size; j++) {
            output.at<cv::Vec3b>(cv::Point(points.at(i).x-j, points.at(i).y-j)) = color;
            output.at<cv::Vec3b>(cv::Point(points.at(i).x+j, points.at(i).y-j)) = color;
            output.at<cv::Vec3b>(cv::Point(points.at(i).x-j, points.at(i).y+j)) = color;
            output.at<cv::Vec3b>(cv::Point(points.at(i).x+j, points.at(i).y+j)) = color;
        }
    }

    return output;
}

// Draw an image-length horizontal line at input row
cv::Mat Segmentation::DrawRow(const cv::Mat& input, const int& row, const cv::Vec3b& color) {
    cv::Mat output;

    input.copyTo(output);

    cv::line(output,
             cv::Point(0, row),
             cv::Point(output.cols-1, row),
             color);

    return output;
}

// Display draw image
void Segmentation::ShowDisplayImage() {
    cv::imshow("Display Image", _display_image);
}

