#include "sensors/euroc_imu_reader.h"
#include <iostream>
#include <sstream>
#include <fstream>

EurocImuReader::EurocImuReader(const std::string& dataset_path)
    : current_index_(0),
      dataset_path_(dataset_path)
{}

void EurocImuReader::open() {
    std::string csv_imu = dataset_path_ + "/mav0/imu0/data.csv";

    std::ifstream f_imu(csv_imu);

    if(!f_imu.is_open()){
        throw std::runtime_error("Failed to open IMU CSV at: " + dataset_path_);
    }

    std::string line_imu;

    // skip header lines
    std::getline(f_imu, line_imu);
    size_t line_idx = 1;

    while(std::getline(f_imu, line_imu)){
        ++line_idx;
        std::stringstream ss_imu(line_imu);

        std::string timestamp_str;
        std::string gyro_x_str, gyro_y_str, gyro_z_str;
        std::string accel_x_str, accel_y_str, accel_z_str;

        std::getline(ss_imu, timestamp_str, ',');
        std::getline(ss_imu, gyro_x_str, ',');
        std::getline(ss_imu, gyro_y_str, ',');
        std::getline(ss_imu, gyro_z_str, ',');
        std::getline(ss_imu, accel_x_str, ',');
        std::getline(ss_imu, accel_y_str, ',');
        std::getline(ss_imu, accel_z_str);

        ImuSample imu_sample;

        try{
            // parse as nanoseconds
            imu_sample.timestamp = std::stoll(timestamp_str);

            imu_sample.gyro_x = std::stod(gyro_x_str);
            imu_sample.gyro_y = std::stod(gyro_y_str);
            imu_sample.gyro_z = std::stod(gyro_z_str);
            imu_sample.accel_x = std::stod(accel_x_str);
            imu_sample.accel_y = std::stod(accel_y_str);
            imu_sample.accel_z = std::stod(accel_z_str);

            entries_.push_back(imu_sample);
        } catch (const std::exception& e){
            throw std::runtime_error(
                "Malformed line " + std::to_string(line_idx) + " in " + csv_imu + 
                " (" + e.what() + "): " + line_imu
            );
        }
    }

    std::cout << "Loaded " << entries_.size() << " IMU samples\n";
}

std::optional<ImuSample> EurocImuReader::read_next() {
    if(current_index_ >= entries_.size()){
        return std::nullopt;
    }

    return entries_[current_index_++];
}

bool EurocImuReader::is_open() const {
    return current_index_ < entries_.size();
}