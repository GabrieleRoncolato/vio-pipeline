#include "frontend/detection/detector_factory.h"
#include "frontend/detection/fast_detector.h"
#include <gtest/gtest.h>

TEST(DetectorFactory, BuildsFastDetectorFromFastConfig) {
    FastConfig cfg{ .threshold = 20 };
    auto detector = make_detector(cfg);
    ASSERT_NE(detector, nullptr);
}

TEST(DetectorFactory, ReturnsConcreteFastDetector) {
    FastConfig cfg{ .threshold = 20 };
    auto detector = make_detector(cfg);
    EXPECT_NE(dynamic_cast<FastDetector*>(detector.get()), nullptr);
}