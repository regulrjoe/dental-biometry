#include "tracing.h"

cv::Mat Tracing::Process(const cv::Mat& input) {
    input.copyTo(_image);
}

void Tracing::FindFirstContourPixel() {

}
