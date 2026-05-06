#include "frontend/selection/selector_factory.h"
#include "frontend/selection/bucketer_selector.h"
#include <gtest/gtest.h>

TEST(SelectorFactory, BuildsBucketerSelectorFromBucketerConfig) {
    BucketerConfig cfg{ 
        .image_width = 160,
        .image_height = 160,
        .cell_width = 20,
        .cell_height = 20,
        .keypoints_per_cell = 20
    };
    auto selector = make_selector(cfg);
    ASSERT_NE(selector, nullptr);
}

TEST(SelectorFactory, ReturnsConcreteBucketerSelector) {
    BucketerConfig cfg{ 
        .image_width = 160,
        .image_height = 160,
        .cell_width = 20,
        .cell_height = 20,
        .keypoints_per_cell = 20
    };
    auto selector = make_selector(cfg);
    EXPECT_NE(dynamic_cast<BucketerSelector*>(selector.get()), nullptr);
}