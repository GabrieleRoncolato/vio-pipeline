#pragma once

#include "sensors/imu_reader.h"
#include "types/imu_sample.h"
#include <string>
#include <vector>

class EurocImuReader: public ImuReader {
public:
    EurocImuReader(const std::string& dataset_path);

    std::optional<ImuSample> read_next() override;
    void open() override;
    bool is_open() const override;
    
private:
    std::string dataset_path_;
    std::vector<ImuSample> entries_;
    size_t current_index_;
};