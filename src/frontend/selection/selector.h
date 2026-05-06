#pragma once

#include "types/keypoint_track.h"
#include <vector>

class Selector {
public:
    virtual ~Selector() = default;
    virtual std::vector<KeyPointTrack> select(const std::vector<KeyPointTrack>& keypoints) const = 0;
};