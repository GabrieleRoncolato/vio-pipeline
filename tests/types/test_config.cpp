#include <gtest/gtest.h>
#include "types/config.h"

namespace {

constexpr const char* kValidYaml = R"(%YAML:1.0
---
data:
  dataset_path: "/some/path"
pipeline:
  tracker: { max_features: 200, fast_threshold: 20 }
  estimator: { ransac_threshold: 1.5, ransac_prob: 0.99 }
  backend: { imu_accel_noise: 0.01, imu_gyro_noise: 0.001 }
)";

} // namespace

TEST(Config, ParsesDatasetPath) {
    Config c = Config::fromYamlString(kValidYaml);
    EXPECT_EQ(c.dataset_path, "/some/path");
}

TEST(Config, LoadsFromFile) {
    Config c = Config::fromYaml(std::string(FIXTURES_DIR) + "/configs/valid.yaml");
    EXPECT_EQ(c.dataset_path, "/some/path");
}

TEST(Config, ThrowsOnMissingFile) {
    EXPECT_THROW(Config::fromYaml("/invalid/yaml/path.yaml"), std::runtime_error);
}

TEST(Config, ThrowsOnMalformedYaml) {
    EXPECT_THROW(Config::fromYamlString("invalid yaml content"), std::runtime_error);
}

TEST(Config, ThrowsOnEmptyString) {
    EXPECT_THROW(Config::fromYamlString(""), std::runtime_error);
}