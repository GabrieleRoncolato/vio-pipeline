#pragma once

#include "frontend/selection/selector.h"
#include "types/config.h"

class BucketerSelector : public Selector {
public:
    explicit BucketerSelector(BucketerConfig bucketer_config);

    std::vector<KeyPointTrack> select(const std::vector<KeyPointTrack>&) const override;

private:
    const int cell_width_;
    const int cell_height_;
    const size_t keypoints_per_cell_;
    const int cell_rows_;
    const int cell_cols_;
};