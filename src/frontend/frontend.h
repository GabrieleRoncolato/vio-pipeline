#pragma once

#include "frontend/detection/detector.h"
#include "frontend/selection/selector.h"
#include "types/frame.h"
#include <memory>

class Frontend {
public:
    explicit Frontend(std::unique_ptr<Detector> detector, std::unique_ptr<Selector> selector);
    void process(const Frame& frame) const;
private:
    std::unique_ptr<Detector> detector_;
    std::unique_ptr<Selector> selector_;
};