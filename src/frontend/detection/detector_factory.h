#pragma once

#include "types/config.h"
#include "frontend/detection/detector.h"
#include <memory>

std::unique_ptr<Detector> make_detector(const DetectorConfig& cfg);