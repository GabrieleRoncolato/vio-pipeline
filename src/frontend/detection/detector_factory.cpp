#include "frontend/detection/detector_factory.h"
#include "frontend/detection/fast_detector.h"
#include "utils/overloaded.h"
#include <variant>

std::unique_ptr<Detector> make_detector(const DetectorConfig& cfg) {
    return std::visit(overloaded {
        [](const FastConfig& c) -> std::unique_ptr<Detector> {
            return std::make_unique<FastDetector>(c);
        },
    }, cfg);
}