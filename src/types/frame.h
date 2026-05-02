#pragma once

#include <opencv2/core.hpp>
#include <cstdint>

struct Frame{
    int64_t timestamp;
    cv::Mat left_image;
    cv::Mat right_image;
};