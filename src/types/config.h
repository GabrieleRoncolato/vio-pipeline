#pragma once

#include <string>

struct TrackerConfig{
    int max_features;
    int fast_threshold;
};

struct EstimatorConfig{
    double ransac_threshold;
    double ransac_prob;
};

struct BackendConfig{
    double imu_accel_noise;
    double imu_gyro_noise;
};

struct Config{
    std::string dataset_path;
    TrackerConfig tracker;
    EstimatorConfig estimator;
    BackendConfig backend;

    static Config fromYaml(const std::string& path);
    static Config fromYamlString(const std::string& yaml_content);
};