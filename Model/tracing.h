#ifndef TRACING_H
#define TRACING_H

#include <iostream>
#include <opencv2/core.hpp>

using namespace std;

class Tracing
{
public:
    // Empty default constructor
    Tracing() {
        cout << "Created instance of Tracing." << endl;
    }

    // Destructor
    ~Tracing() {
        cout << "Destroyed instance of Tracing." << endl;
    }

    // Run algorithm
    cv::Mat Process(const cv::Mat&);

    //// SETTERS AND GETTERS ////


private:
    //// INTERNAL OBJECTS ////
    // Local copy of input image for processing
    cv::Mat _image;
    // Local copy of _image for drawing and displaying
    cv::Mat _display_image;
    // Vector of points in resulting contour of tooth
    vector<cv::Point> _contour;
    // Vector of slopes at each point in contour
    vector<float> _slopes;
    // Vector of angles at each point in contour
    vector<float> _angles;

    //// PARAMETERS ////


    //// METHODS ////
    // Find the first pixel from where the tracing starts
    void FindFirstContourPixel();
};

#endif // TRACING_H
