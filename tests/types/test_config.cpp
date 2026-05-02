#include <gtest/gtest.h>
#include "types/config.h"

namespace {

constexpr const char* k_valid_yaml = R"(%YAML:1.0
---
data:
  dataset_path: "/some/path"
pipeline:
  frontend:
    detector: { type: fast, threshold: 20 }
)";

} // namespace

TEST(Config, ParsesDatasetPath) {
    Config c = Config::from_yaml_string(k_valid_yaml);
    EXPECT_EQ(c.dataset_path, "/some/path");
}

TEST(Config, LoadsFromFile) {
    Config c = Config::from_yaml(std::string(FIXTURES_DIR) + "/configs/valid.yaml");
    EXPECT_EQ(c.dataset_path, "/some/path");
}

TEST(Config, ThrowsOnMissingFile) {
    EXPECT_THROW(Config::from_yaml("/invalid/yaml/path.yaml"), std::runtime_error);
}

TEST(Config, ThrowsOnMalformedYaml) {
    EXPECT_THROW(Config::from_yaml_string("invalid yaml content"), std::runtime_error);
}

TEST(Config, ThrowsOnEmptyString) {
    EXPECT_THROW(Config::from_yaml_string(""), std::runtime_error);
}