#pragma once

#include "frontend/detection/detector.h"
#include "types/config.h"

class FastDetector : public Detector {
public:
    explicit FastDetector(FastConfig detector_config);

    std::vector<cv::KeyPoint> detect(const cv::Mat&) const override;

private:
    cv::Ptr<cv::FastFeatureDetector> fast_;
};