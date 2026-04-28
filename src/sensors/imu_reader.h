#pragma once
#include <optional>
#include "types/imu_sample.h"

class ImuReader {
public:
    virtual ~ImuReader() = default;
    virtual std::optional<ImuSample> read_next() = 0;
    virtual bool is_open() const = 0;
};