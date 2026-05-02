#include "frontend/detection/fast_detector.h"

FastDetector::FastDetector(FastConfig fast_config) 
    : fast_(cv::FastFeatureDetector::create(fast_config.threshold))
{}

std::vector<cv::KeyPoint> FastDetector::detect(const cv::Mat& img) const {
    std::vector<cv::KeyPoint> keypoints;
    fast_->detect(img, keypoints);
    return keypoints;
}