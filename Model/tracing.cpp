#include "tracing.h"
#include "helpers.h"
#include "visualizationhelpers.h"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

cv::Mat Tracing::Process(const cv::Mat& input) {
    input.copyTo(_image);
    _display_image = cv::Mat::zeros(input.cols, input.rows, CV_8UC3);
    // Convert from grayscale to RGB for drawing purposes
    cv::cvtColor(input, _display_image, CV_GRAY2RGB, 3);

    // Find the first pixel from the where tracing starts and add it to vectors with slope and angle.
    AddPixelValuesToVectors(
                FindFirstContourPixel(
                    _first_pixel_intensity_threshold,
                    _first_pixel_inner_margin));

    // Start off form the first pixel and trace the crown of the tooth down to the neck
    TraceCrown(_crown_trace_max_pct_height, _crown_trace_extrapolation_distance, _crown_trace_extrapolation_mask);

    cv::imshow("display_image", _display_image);

    return _image;

}

// Find the first pixel from where the tracing starts.
cv::Point Tracing::FindFirstContourPixel(const int& intensity_thr, const int& inner_margin) {
    int x, y;

    for (y = inner_margin; y < _image.rows - inner_margin; y++)
        for (x = inner_margin; x < _image.cols - inner_margin; x++)
            if (_image.at<uchar>(cv::Point(x, y)) >= intensity_thr)
                goto STOP;
    STOP:

    return cv::Point(x, y);
}

// Trace the crown of the tooth down to the neck
void Tracing::TraceCrown(const float& max_height_pct, const int& extrapolation_distance, const int& extrapolation_mask) {
    cv::Point extrapolated;
    int max_height;
    int counter;

    max_height = max_height_pct * _image.rows;

    // Trace down the left side
    counter = 0;
    do {
        // First contour pixels are based off brithness to gain intertia in a particular direction.
        if (counter <= 5) {
            // Find the brightest pixel in the bottom-left neighborhood.
            AddPixelValuesToVectors(
                        BrightestPixelInNeighborhood(
                            _contour.back(),
                            false,      // top
                            false,      // right
                            true,       // left
                            true));     // bottom
        } else {
            // Extrapolate based on the last pixel's angle.
            extrapolated = Helpers::LinearExtrapolation(_contour.back(), _angles.back(), extrapolation_distance);

            // Make sure extrapolated pixel is inside image
            if (extrapolated.x < extrapolation_mask / 2
                    || extrapolated.x > _image.cols - extrapolation_mask / 2
                    || extrapolated.y < extrapolation_mask / 2
                    || extrapolated.y > _image.rows - extrapolation_mask / 2)
                break;

            // Add the fittest pixel in the extrapolated pixel's mask
            AddPixelValuesToVectors(
                        FittestPixelInMask(
                            extrapolated,
                            extrapolation_mask));
        }
        counter++;

        _display_image.at<cv::Vec3b>(_contour.back()) = cv::Vec3b(255, 255, 255);
        cv::imshow("display_image", _display_image);
        cv::waitKey(0);
    } while (_contour.back().x < max_height);

    // Reverse all vector so the beginning of the right side trace appends to the left side trace.
    reverse(_contour.begin(), _contour.end());
    reverse(_slopes.begin(), _slopes.end());
    reverse(_angles.begin(), _angles.end());

    // Trace down the right side
    counter = 0;
    do {
        // First contour pixels are based off neighborhood brithness to gain intertia in a particular direction.
        if (counter <= 5) {
            // Find the brightest pixel in the bottom-right neighborhood.
            AddPixelValuesToVectors(
                        BrightestPixelInNeighborhood(
                            _contour.back(),
                            false,      // top
                            true,       // right
                            true,       // bottom
                            false));    // left
        } else {
            // Extrapolate based on the last pixel's angle
            extrapolated = Helpers::LinearExtrapolation(_contour.back(), _angles.back(), extrapolation_distance);

            // Make sure extrapolated pixel is inside image
            if (extrapolated.x < extrapolation_mask / 2
                    || extrapolated.x > _image.cols - extrapolation_mask / 2
                    || extrapolated.y < extrapolation_mask / 2
                    || extrapolated.y > _image.rows - extrapolation_mask / 2)
                break;

            // Add the fittest pixel in the extrapolated pixel's mask
            AddPixelValuesToVectors(
                        FittestPixelInMask(
                            extrapolated,
                            extrapolation_mask));
        }
        counter++;

        _display_image.at<cv::Vec3b>(_contour.back()) = cv::Vec3b(255, 255, 255);
        cv::imshow("display_image", _display_image);
        cv::waitKey(0);
    } while (_contour.back().x < max_height);
}

// From input pixel obtain slope and angle, and append all to their respective vectors.
void Tracing::AddPixelValuesToVectors(const cv::Point& pixel) {
    _contour.push_back(pixel);

    if ((int)_contour.size() == 1) {
        // First element cannot have slope or angle.
        _slopes.push_back(0);
        _angles.push_back(0);
    } else if ((int)_contour.size() <= _slope_angle_distance) {
        // Measure slope and angle from the last pixel to the first.
        _slopes.push_back(
                    Helpers::GetSlope(
                        _contour.front(),
                        _contour.back()));
        _angles.push_back(
                    Helpers::GetAngle(
                        _contour.front(),
                        _contour.back(),
                        false));
    } else {
        // Measure slope and angle from the last pixel to the pixel at _slope_angle_distance.
        _slopes.push_back(
                    Helpers::GetSlope(
                        _contour.rbegin()[_slope_angle_distance],
                        _contour.back()));
        _angles.push_back(
                    Helpers::GetAngle(
                        _contour.rbegin()[_slope_angle_distance],
                        _contour.back(),
                        false));
    }
}

// Find the brightest pixel (not already in _contour) in the neighborhood of input pixel.
// Each boolean represents the side of the neighborhood where the search is done.
cv::Point Tracing::BrightestPixelInNeighborhood(const cv::Point& center_pixel, const bool& top, const bool& right, const bool& bottom, const bool& left) {
    cv::Point   brightest_pixel,
                current_pixel;
    int     brightest_value,
            current_brightness;
    int x, y;

    brightest_value = 0;

    if (top) {
        for (x = -1; x < 2; x++) {
            current_pixel = cv::Point(center_pixel.x + x, center_pixel.y - 1);
            if (Helpers::HasPoint(current_pixel, _contour))
                break;

            current_brightness = _image.at<uchar>(current_pixel);
            if (current_brightness > brightest_value) {
                brightest_value = current_brightness;
                brightest_pixel = current_pixel;
            }
        }
    }

    if (right) {
        for (y = -1; y < 2; y++) {
            current_pixel = cv::Point(center_pixel.x + 1, center_pixel.y + y);
            if (Helpers::HasPoint(current_pixel, _contour))
                break;

            current_brightness = _image.at<uchar>(current_pixel);
            if (current_brightness > brightest_value) {
                brightest_value = current_brightness;
                brightest_pixel = current_pixel;
            }
        }
    }

    if (bottom) {
        for (x = -1; x < 2; x++) {
            current_pixel = cv::Point(center_pixel.x + x, center_pixel.y + 1);
            if (Helpers::HasPoint(current_pixel, _contour))
                break;

            current_brightness = _image.at<uchar>(current_pixel);
            if (current_brightness > brightest_value) {
                brightest_value = current_brightness;
                brightest_pixel = current_pixel;
            }
        }
    }

    if (left) {
        for (y = -1; y < 2; y++) {
            current_pixel = cv::Point(center_pixel.x - 1, center_pixel.y + y);
            if (Helpers::HasPoint(current_pixel, _contour))
                break;

            current_brightness = _image.at<uchar>(current_pixel);
            if (current_brightness > brightest_value) {
                brightest_value = current_brightness;
                brightest_pixel = current_pixel;
            }
        }
    }

    return brightest_pixel;
}


// Get the fittest pixel inside a KxK mask.
// The fittest pixel is that with the max value of: its brightness - avg brigthness of its neighbors.
// This is to find the brightest pixel at the edge of the border.
cv::Point Tracing::FittestPixelInMask(const cv::Point& center_pixel, const int& k_size) {
    cv::Point fittest_pixel;
    cv::Point current_pixel;

    int current_brightness;
    float current_fitness;
    float fittest_value;
    int x, y;

    // Start off with a minimum value
    fittest_value = -1000;

    for (x = -1 * (k_size / 2); x <= (k_size / 2); x++) {
        for (y = -1 * (k_size / 2); y <= (k_size / 2); y++) {

            current_pixel = cv::Point(center_pixel.x + x, center_pixel.y + y);

            if (Helpers::HasPoint(current_pixel, _contour))
                break;

            current_brightness = _image.at<uchar>(current_pixel);
            current_fitness = current_brightness
                    - (Helpers::SumOfNeighbors(_image, current_pixel, k_size)
                       / ((k_size * k_size) - 1));

            if (current_fitness > fittest_value) {
                fittest_value = current_fitness;
                fittest_pixel = current_pixel;
            }
        }
    }

    return fittest_pixel;
}
