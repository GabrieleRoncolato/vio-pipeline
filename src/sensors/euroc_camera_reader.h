#pragma once

#include "sensors/camera_reader.h"
#include "types/frame.h"
#include <cstdint>
#include <string>
#include <vector>

struct CameraEntry{
    int64_t timestamp;
    std::string left_path;
    std::string right_path;
};

class EurocCameraReader: public CameraReader{
public:
    EurocCameraReader(const std::string& dataset_path);

    std::optional<Frame> read_next() override;
    bool is_open() const override;
    
private:
    std::vector<CameraEntry> entries;
    size_t current_index;

    void load_entries(const std::string& dataset_path);
};