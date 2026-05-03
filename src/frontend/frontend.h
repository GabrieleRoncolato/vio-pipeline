#pragma once

#include "types/config.h"
#include "frontend/detection/detector.h"
#include "types/frame.h"
#include <memory>

class Frontend {
public:
    explicit Frontend(std::unique_ptr<Detector> detector);
    void process(const Frame& frame) const;
private:
    std::unique_ptr<Detector> detector_;
};