#include "Processor.h"
#include <opencv2/imgproc.hpp>
#include <vector>

Processor::Processor() {
    // Pre-allocate matrices if sizes are known, or they will be allocated on first frame
}

Processor::~Processor() {
    // Cleanup
}

cv::Mat Processor::process_frame(int width, int height,
                                 const uint8_t* y_data, int y_row_stride,
                                 const uint8_t* u_data, int u_row_stride, int u_pixel_stride,
                                 const uint8_t* v_data, int v_row_stride, int v_pixel_stride) {

    // Create a Mat header for the Y plane
    cv::Mat y_mat(height, width, CV_8UC1, (void*)y_data, y_row_stride);

    // For Canny, we only need the grayscale (Y) channel.
    // If we needed color, we would have to convert from YUV to RGB.
    // cv::cvtColor(yuv_mat, rgb_mat, cv::COLOR_YUV2RGB_I420);

    // Apply Canny edge detection
    // The thresholds 50 and 150 are common starting points.
    cv::Canny(y_mat, canny_mat, 50, 150);

    // Canny output is a single-channel (grayscale) image.
    // To display it with OpenGL as an RGB texture, we need to convert it.
    cv::cvtColor(canny_mat, gray_mat, cv::COLOR_GRAY2RGBA, 4);

    return gray_mat;
}
