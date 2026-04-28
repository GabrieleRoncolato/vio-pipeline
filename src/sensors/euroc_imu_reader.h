#pragma once

#include "sensors/imu_reader.h"
#include "types/imu_sample.h"
#include <string>
#include <vector>

class EurocImuReader: public ImuReader {
public:
    EurocImuReader(const std::string& dataset_path);

    std::optional<ImuSample> read_next() override;
    bool is_open() const override;
    
private:
    std::vector<ImuSample> entries;
    size_t current_index;

    void load_entries(const std::string& dataset_path);
};