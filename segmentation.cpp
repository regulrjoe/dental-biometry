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

    _display_image = DrawXAtPoints(_image, _crowns.first, cv::Vec3b(0, 0, 255));
    _display_image = DrawXAtPoints(_image, _crowns.second, cv::Vec3b(255, 0, 0));

    ShowDisplayImage();

    return _image;
}

// Obtain vertical line profiles of image
// INPUT: img -> image from where line profiles are obtained
// INPUT: sp -> column spacing between profiles
// INPUT: dd -> Derivative distance between values
// OUTPUT: vector of pairs <column, profile>
vector <pair <int, vector<int> > > Segmentation::DerivativeLineProfiles(const cv::Mat& img, const int& sp, const int& dd) {
    int c; // column iterator
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

    // Obtain maximum and minimum values of each line profile
    int i,
        col,
        max_value_row,
        min_value_row;

    for (i = 0; i < (int)line_profiles.size(); i++) {
        col = line_profiles.at(i).first;
        max_value_row = Helpers::MaxValueIndex(line_profiles.at(i).second, -1, -1);
        min_value_row = Helpers::MinValueIndex(line_profiles.at(i).second, -1, -1);

        // Maximum value's row must above minimum value's row to be valid
        if (max_value_row < min_value_row ) {
            _crowns.first.push_back(cv::Point(col, max_value_row));
            _crowns.second.push_back(cv::Point(col, min_value_row));
        }
    }
}

// Mark an X at input point
// INPUT: img -> image to draw on
// INPUT: points -> vector of points to draw
// INPUT: color -> color of X marks
// INPUT: x_size -> lenght of X to draw
// OUTPUT: image with points marked
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

// Display draw image
void Segmentation::ShowDisplayImage() {
    cv::imshow("Display Image", _display_image);
}

