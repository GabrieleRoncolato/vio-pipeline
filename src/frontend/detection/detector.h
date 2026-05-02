#pragma once

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

class Detector {
public:
    virtual ~Detector() = default;
    virtual std::vector<cv::KeyPoint> detect(const cv::Mat& img) const = 0;
};