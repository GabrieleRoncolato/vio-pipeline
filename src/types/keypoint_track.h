#pragma once

#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <cstdint>

struct KeyPointTrack {
    std::int64_t id;
    size_t duration;
    cv::KeyPoint prev_keypoint;
    cv::KeyPoint curr_keypoint;
};