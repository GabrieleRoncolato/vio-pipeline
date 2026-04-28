#pragma once

#include <opencv2/core.hpp>
#include <cstdint>

struct Frame{
    int64_t timestamp;
    cv::Mat right_image;
    cv::Mat left_image;
};