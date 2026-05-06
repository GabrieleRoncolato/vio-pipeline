#include "frontend/selection/bucketer_selector.h"
#include "types/keypoint_track.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

BucketerSelector::BucketerSelector(
    BucketerConfig bucketer_config
) :
    cell_width_(bucketer_config.cell_width),
    cell_height_(bucketer_config.cell_height),
    keypoints_per_cell_(bucketer_config.keypoints_per_cell),
    cell_rows_(bucketer_config.image_height / bucketer_config.cell_height),
    cell_cols_(bucketer_config.image_width / bucketer_config.cell_width)
{
    auto image_width = bucketer_config.image_width;
    auto image_height = bucketer_config.image_height;

    if(image_width % cell_width_ != 0 || image_height % cell_height_ != 0){
        throw std::runtime_error(
            "Image width and image height ("
            + std::to_string(image_width) + "x" + std::to_string(image_height)
            + ") must be divisible by cell width and cell height ("
            + std::to_string(cell_width_) + "x" + std::to_string(cell_height_) + ")"
        );
    }
}

std::vector<KeyPointTrack> BucketerSelector::select(const std::vector<KeyPointTrack>& keypoint_tracks) const {
    if(keypoint_tracks.empty()){
        return {};
    }

    const auto num_cells = cell_rows_ * cell_cols_;
    std::vector<std::vector<KeyPointTrack>> buckets(num_cells);

    std::vector<KeyPointTrack> output_keypoint_tracks;
    output_keypoint_tracks.reserve(num_cells * keypoints_per_cell_);

    for(const auto& keypoint_track : keypoint_tracks){
        const auto coord_x = keypoint_track.curr_keypoint.pt.x;
        const auto coord_y = keypoint_track.curr_keypoint.pt.y;

        auto cell_x = static_cast<int>(coord_x) / cell_width_;
        auto cell_y = static_cast<int>(coord_y) / cell_height_;

        cell_x = std::clamp(cell_x, 0, cell_cols_ - 1);
        cell_y = std::clamp(cell_y, 0, cell_rows_ - 1);
        const auto bucket_idx = cell_y * cell_cols_ + cell_x;

        buckets[bucket_idx].push_back(keypoint_track);
    }

    for(auto& bucket : buckets){
        if(bucket.empty()){
            continue;
        }

        if(bucket.size() > keypoints_per_cell_){
            std::partial_sort(bucket.begin(), bucket.begin() + keypoints_per_cell_, bucket.end(),
                [](const KeyPointTrack& a, const KeyPointTrack& b) {
                return a.duration > b.duration 
                    || (a.duration == b.duration && a.curr_keypoint.response > b.curr_keypoint.response);
            });
        }

        const size_t keep = std::min(keypoints_per_cell_, bucket.size());
        output_keypoint_tracks.insert(output_keypoint_tracks.end(), bucket.begin(), bucket.begin() + keep);
    }

    return output_keypoint_tracks;
}