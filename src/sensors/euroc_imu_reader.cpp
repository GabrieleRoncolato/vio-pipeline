#include "sensors/euroc_imu_reader.h"
#include <iostream>
#include <sstream>
#include <fstream>

EurocImuReader::EurocImuReader(const std::string& dataset_path)
    : current_index(0)
{
    load_entries(dataset_path);
}

void EurocImuReader::load_entries(const std::string& dataset_path) {
    std::string csv_imu = dataset_path + "/mav0/imu0/data.csv";

    std::ifstream f_imu(csv_imu);

    if(!f_imu.is_open()){
        std::cerr << "Failed to open IMU CSV at: " << dataset_path << "\n";
        return;
    }

    std::string line_imu;

    // skip header lines
    std::getline(f_imu, line_imu);

    while(std::getline(f_imu, line_imu)){
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

        // parse as nanoseconds
        imu_sample.timestamp = std::stoll(timestamp_str);

        imu_sample.gyro_x = std::stod(gyro_x_str);
        imu_sample.gyro_y = std::stod(gyro_y_str);
        imu_sample.gyro_z = std::stod(gyro_z_str);
        imu_sample.accel_x = std::stod(accel_x_str);
        imu_sample.accel_y = std::stod(accel_y_str);
        imu_sample.accel_z = std::stod(accel_z_str);

        entries.push_back(imu_sample);
    }

    std::cout << "Loaded " << entries.size() << " IMU samples\n";
}

std::optional<ImuSample> EurocImuReader::read_next() {
    if(current_index >= entries.size()){
        return std::nullopt;
    }

    return entries[current_index++];
}

bool EurocImuReader::is_open() const {
    return current_index < entries.size();
}