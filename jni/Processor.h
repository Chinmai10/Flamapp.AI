#ifndef FLAMAPP_PROCESSOR_H
#define FLAMAPP_PROCESSOR_H

#include <opencv2/core.hpp>

class Processor {
public:
    Processor();
    ~Processor();

    cv::Mat process_frame(int width, int height,
                          const uint8_t* y_data, int y_row_stride,
                          const uint8_t* u_data, int u_row_stride, int u_pixel_stride,
                          const uint8_t* v_data, int v_row_stride, int v_pixel_stride);

private:
    cv::Mat yuv_mat;
    cv::Mat gray_mat;
    cv::Mat canny_mat;
};

#endif //FLAMAPP_PROCESSOR_H
