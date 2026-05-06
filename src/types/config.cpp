#include "types/config.h"
#include <opencv2/core.hpp>
#include <stdexcept>

namespace {

template<typename T>
T read_required(const cv::FileNode& n, const char* key){
    auto sub = n[key];
    if(sub.empty())
        throw std::runtime_error(std::string("Missing required config key: ") + key);
    return (T)sub;
}

cv::FileNode read_section(const cv::FileNode& n, const char* key){
    auto sub = n[key];
    if(sub.empty())
        throw std::runtime_error(std::string("Missing required config section: ") + key);
    return sub;
}

DetectorConfig parse_detector(cv::FileNode node){
    auto type = read_required<std::string>(node, "type");

    if(type == "fast"){
        FastConfig config;
        config.threshold = read_required<int>(node, "threshold");
        return config;
    }

    throw std::runtime_error("Unknown detector type: " + type);
}

SelectorConfig parse_selector(cv::FileNode node){
    auto type = read_required<std::string>(node, "type");

    if(type == "bucketer"){
        BucketerConfig config;
        config.image_width = read_required<int>(node, "image_width");
        config.image_height = read_required<int>(node, "image_height");
        config.cell_width = read_required<int>(node, "cell_width");
        config.cell_height = read_required<int>(node, "cell_height");
        config.keypoints_per_cell = read_required<int>(node, "keypoints_per_cell");
        return config;
    }

    throw std::runtime_error("Unknown selector type: " + type);
}

FrontendConfig parse_frontend(cv::FileNode node){
    FrontendConfig frontend_config;
    
    auto detector = read_section(node, "detector");
    frontend_config.detector_config = parse_detector(detector);
    auto selector = read_section(node, "selector");
    frontend_config.selector_config = parse_selector(selector);
    
    return frontend_config;
}

Config parse(cv::FileStorage& fs){
    Config config;
    
    auto data = read_section(fs.root(), "data");
    config.dataset_path = read_required<std::string>(data, "dataset_path");

    auto pipeline = read_section(fs.root(), "pipeline");
    auto frontend = read_section(pipeline, "frontend");
    config.frontend_config = parse_frontend(frontend);

    return config;
}

cv::FileStorage open_file_storage(const std::string& source, int flags, const std::string& context){
    cv::FileStorage fs;

    try{
        fs.open(source, flags);
    } catch (const cv::Exception& e){
        throw std::runtime_error(context + ": " + e.what());
    }

    if(!fs.isOpened()){
        throw std::runtime_error(context);
    }

    return fs;
}

} // namespace

Config Config::from_yaml(const std::string& path){
    cv::FileStorage fs = open_file_storage(
        path, 
        cv::FileStorage::READ, 
        "Config file not found or unreadable: " + path
    );
    return parse(fs);
}

Config Config::from_yaml_string(const std::string& yaml_content){
    cv::FileStorage fs = open_file_storage(
        yaml_content, 
        cv::FileStorage::READ | cv::FileStorage::MEMORY,
        "Failed to parse YAML string"
    );
    return parse(fs);
}