#include "sensors/euroc_camera_reader.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

EurocCameraReader::EurocCameraReader(const std::string& dataset_path)
    : current_index(0)
{
    load_entries(dataset_path);
}

void EurocCameraReader::load_entries(const std::string& dataset_path) {
    std::string csv_left = dataset_path + "/mav0/cam0/data.csv";
    std::string csv_right = dataset_path + "/mav0/cam1/data.csv";

    std::ifstream f_left(csv_left);
    std::ifstream f_right(csv_right);

    if(!f_left.is_open() || !f_right.is_open()){
        std::cerr << "Failed to open camera CSVs at: " << dataset_path << "\n";
        return;
    }

    std::string line_left, line_right;

    // skip header lines
    std::getline(f_left, line_left);
    std::getline(f_right, line_right);

    while(std::getline(f_left, line_left) && std::getline(f_right, line_right)){
        std::stringstream ss_left(line_left);
        std::string timestamp_str, filename_left, filename_right;

        std::getline(ss_left, timestamp_str, ',');
        std::getline(ss_left, filename_left);

        std::stringstream ss_right(line_right);
        std::string dummy;
        std::getline(ss_right, dummy, ',');
        std::getline(ss_right, filename_right);

        CameraEntry entry;

        // parse as nanoseconds
        entry.timestamp = std::stoll(timestamp_str);

        entry.left_path = dataset_path + "/mav0/cam0/data/" + filename_left;
        entry.right_path = dataset_path + "/mav0/cam1/data/" + filename_right;

        entries.push_back(entry);
    }

    std::cout << "Loaded " << entries.size() << " stereo pairs\n";
}

std::optional<Frame> EurocCameraReader::read_next() {
    if(current_index >= entries.size()){
        return std::nullopt;
    }

    const auto& entry = entries[current_index++];

    Frame frame;
    frame.timestamp = entry.timestamp;
    frame.left_image = cv::imread(entry.left_path, cv::IMREAD_GRAYSCALE);
    frame.right_image = cv::imread(entry.right_path, cv::IMREAD_GRAYSCALE);

    if(frame.left_image.empty() || frame.right_image.empty()){
        std::cerr << "Failed to load images at index: " << current_index << "\n";
        return std::nullopt;
    }

    return frame;
}

bool EurocCameraReader::is_open() const {
    return current_index < entries.size();
}