#include "frontend/selection/bucketer_selector.h"
#include "types/config.h"
#include "types/keypoint_track.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace{

KeyPointTrack make_track(std::int64_t id, float x, float y, float response, size_t duration) {
    cv::KeyPoint keypoint(x, y, 1.0f, -1.0f, response);
    return KeyPointTrack{ id, duration, keypoint, keypoint };
}

BucketerConfig make_config(int image_w, int image_h, int cell_w, int cell_h, int k){
    BucketerConfig config;
    config.image_width = image_w;
    config.image_height = image_h;
    config.cell_width = cell_w;
    config.cell_height = cell_h;
    config.keypoints_per_cell = k;
    return config;
}

} // namespace

TEST(BucketerSelector, SelectsEmptyOnEmptyInput) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 2));
    EXPECT_TRUE(selector.select({}).empty());
}

TEST(BucketerSelector, SelectsAllWhenBucketFree) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 3));
    std::vector<KeyPointTrack> input = {
        make_track(0, 10, 10, 0.5f, 0),
        make_track(1, 20, 20, 0.5f, 0)
    };
    
    EXPECT_EQ(selector.select(input).size(), 2u);
}

TEST(BucketerSelector, SelectsKWhenBucketFull) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 2));
    std::vector<KeyPointTrack> input = {
        make_track(0, 10, 10, 0.1f, 0),
        make_track(1, 11, 11, 0.2f, 0),
        make_track(2, 12, 12, 0.3f, 0),
        make_track(3, 13, 13, 0.4f, 0),
        make_track(4, 14, 14, 0.5f, 0),
    };
    
    EXPECT_EQ(selector.select(input).size(), 2u);
}

TEST(BucketerSelector, SelectsByDurationDescending) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 1));
    std::vector<KeyPointTrack> input = {
        make_track(0, 10, 10, 1.0f, 0),
        make_track(1, 20, 20, 0.1f, 5)
    };

    auto selected_keypoints = selector.select(input);
    ASSERT_EQ(selected_keypoints.size(), 1u);
    EXPECT_EQ(selected_keypoints[0].duration, 5u);
}

TEST(BucketerSelector, SelectsByResponseDescendingOnTiedDurations) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 1));
    std::vector<KeyPointTrack> input = {
        make_track(0, 10, 10, 0.2f, 2),
        make_track(1, 20, 20, 0.5f, 2)
    };

    auto selected_keypoints = selector.select(input);
    ASSERT_EQ(selected_keypoints.size(), 1u);
    EXPECT_FLOAT_EQ(selected_keypoints[0].curr_keypoint.response, 0.5f);
}

TEST(BucketerSelector, SelectsKPerBucket) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 1));
    std::vector<KeyPointTrack> input = {
        make_track(0, 10, 10, 0.1f, 0),
        make_track(1, 20, 20, 0.5f, 0),

        make_track(2, 70, 10, 0.5f, 0),

        make_track(3, 70, 70, 0.1f, 0),
        make_track(4, 80, 80, 0.2f, 0),
        make_track(5, 90, 90, 0.3f, 0),
    };

    auto selected_keypoints = selector.select(input);
    EXPECT_EQ(selected_keypoints.size(), 3u);
}

TEST(BucketerSelector, ClampsEdgeKeypoints) {
    BucketerSelector selector(make_config(100, 100, 50, 50, 3));
    std::vector<KeyPointTrack> input = {
        make_track(0, 100, 100, 0.5f, 0)
    };

    EXPECT_NO_THROW(selector.select(input));
    EXPECT_EQ(selector.select(input).size(), 1u);
}