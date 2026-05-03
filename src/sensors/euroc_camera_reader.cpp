#include "sensors/euroc_camera_reader.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

EurocCameraReader::EurocCameraReader(const std::string& dataset_path)
    : current_index_(0),
      dataset_path_(dataset_path)
{}

void EurocCameraReader::open() {
    std::string csv_left = dataset_path_ + "/mav0/cam0/data.csv";
    std::string csv_right = dataset_path_ + "/mav0/cam1/data.csv";

    std::ifstream f_left(csv_left);
    std::ifstream f_right(csv_right);

    if(!f_left.is_open() || !f_right.is_open()){
        throw std::runtime_error("Failed to open camera CSVs at: " + dataset_path_);
    }

    std::string line_left, line_right;

    // skip header lines
    std::getline(f_left, line_left);
    std::getline(f_right, line_right);
    size_t line_idx = 1;

    while(true){
        bool got_left  = static_cast<bool>(std::getline(f_left, line_left));
        bool got_right = static_cast<bool>(std::getline(f_right, line_right));

        if(!got_left && !got_right) break;
        if(got_left != got_right){
            throw std::runtime_error(
                "Camera CSV row counts differ between " + csv_left + " and " + csv_right
            );
        }

        if(!line_left.empty()  && line_left.back()  == '\r') line_left.pop_back();
        if(!line_right.empty() && line_right.back() == '\r') line_right.pop_back();

        ++line_idx;
        std::stringstream ss_left(line_left);
        std::string timestamp_str, filename_left, filename_right;

        std::getline(ss_left, timestamp_str, ',');
        std::getline(ss_left, filename_left);

        std::stringstream ss_right(line_right);
        std::string dummy;
        std::getline(ss_right, dummy, ',');
        std::getline(ss_right, filename_right);

        CameraEntry entry;

        try{
            // parse as nanoseconds
            entry.timestamp = std::stoll(timestamp_str);

            entry.left_path = dataset_path_ + "/mav0/cam0/data/" + filename_left;
            entry.right_path = dataset_path_ + "/mav0/cam1/data/" + filename_right;

            entries_.push_back(entry);
        } catch (const std::exception& e){
            throw std::runtime_error(
                "Malformed line " + std::to_string(line_idx) + 
                " in " + csv_left + " (" + e.what() + "): " + line_left
            );
        }
    }
    
    if(std::getline(f_left, line_left) || std::getline(f_right, line_right)){
        throw std::runtime_error("Camera CSV row counts differ between " + csv_left + " and " + csv_right);
    }

    std::cout << "Loaded " << entries_.size() << " stereo pairs\n";
}

std::optional<Frame> EurocCameraReader::read_next() {
    if(current_index_ >= entries_.size()){
        return std::nullopt;
    }

    const auto& entry = entries_[current_index_++];

    Frame frame;
    frame.timestamp = entry.timestamp;
    frame.left_image = cv::imread(entry.left_path, cv::IMREAD_GRAYSCALE);
    frame.right_image = cv::imread(entry.right_path, cv::IMREAD_GRAYSCALE);

    if(frame.left_image.empty() || frame.right_image.empty()){
        throw std::runtime_error("Failed to open camera image at frame index: " + std::to_string(current_index_));
    }

    return frame;
}

bool EurocCameraReader::is_open() const {
    return current_index_ < entries_.size();
}