#pragma once

#include <string>
#include <variant>

struct FastConfig {
    int threshold;
};

using DetectorConfig = std::variant<FastConfig>;

struct FrontendConfig {
    DetectorConfig detector_config;
};

struct Config {
    std::string dataset_path;
    
    FrontendConfig frontend_config;

    static Config from_yaml(const std::string& path);
    static Config from_yaml_string(const std::string& yaml_content);
};