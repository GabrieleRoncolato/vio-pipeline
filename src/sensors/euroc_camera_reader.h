#pragma once

#include "sensors/camera_reader.h"
#include <cstdint>
#include <string>
#include <vector>

class EurocCameraReader: public CameraReader {
public:
    EurocCameraReader(const std::string& dataset_path);

    std::optional<Frame> read_next() override;
    void open() override;
    bool is_open() const override;
    
private:
    struct CameraEntry {
        int64_t timestamp;
        std::string left_path;
        std::string right_path;
    };
    
    std::string dataset_path_;
    std::vector<CameraEntry> entries_;
    size_t current_index_;
};