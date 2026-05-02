#include "frontend/frontend.h"
#include <iostream>

Frontend::Frontend(std::unique_ptr<Detector> detector)
    : detector_(std::move(detector))
{}

void Frontend::process(const Frame& frame) const {
    auto keypoints = detector_->detect(frame.right_image);
    std::cout << "Detected " << std::to_string(keypoints.size()) << "\n";
}