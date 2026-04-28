#include "types/config.h"
#include <opencv2/core.hpp>
#include <stdexcept>

namespace {

Config parse(cv::FileStorage& fs){
    Config config;
    config.dataset_path = (std::string)fs["data"]["dataset_path"];
    config.tracker.max_features = (int)fs["pipeline"]["tracker"]["max_features"];
    config.tracker.fast_threshold = (int)fs["pipeline"]["tracker"]["fast_threshold"];
    config.estimator.ransac_threshold = (double)fs["pipeline"]["estimator"]["ransac_threshold"];
    config.estimator.ransac_prob = (double)fs["pipeline"]["estimator"]["ransac_prob"];
    config.backend.imu_accel_noise = (double)fs["pipeline"]["backend"]["imu_accel_noise"];
    config.backend.imu_gyro_noise = (double)fs["pipeline"]["backend"]["imu_gyro_noise"];
    return config;
}

cv::FileStorage openFileStorage(const std::string& source, int flags, const std::string& context){
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

}

Config Config::fromYaml(const std::string& path){
    cv::FileStorage fs = openFileStorage(
        path, 
        cv::FileStorage::READ, 
        "Config file not found or unreadable: " + path
    );
    return parse(fs);
}

Config Config::fromYamlString(const std::string& yaml_content){
    cv::FileStorage fs = openFileStorage(
        yaml_content, 
        cv::FileStorage::READ | cv::FileStorage::MEMORY,
        "Failed to parse YAML string"
    );
    return parse(fs);
}