#pragma once

#include <cstdint>

struct ImuSample{
    int64_t timestamp;
    double gyro_x;
    double gyro_y;
    double gyro_z;
    double accel_x;
    double accel_y;
    double accel_z;
};