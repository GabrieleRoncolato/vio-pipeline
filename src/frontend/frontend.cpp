#include "frontend/frontend.h"
#include <iostream>
#include <memory>

Frontend::Frontend(std::unique_ptr<Detector> detector, std::unique_ptr<Selector> selector)
    : 
    detector_(std::move(detector)),
    selector_(std::move(selector))
{}

void Frontend::process(const Frame& frame) const {
    auto keypoints = detector_->detect(frame.right_image);

    std::cout << "Detected " << std::to_string(keypoints.size()) << "\n";
}