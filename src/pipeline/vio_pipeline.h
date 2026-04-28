#pragma once

#include <memory>
#include "sensors/camera_reader.h"
#include "sensors/imu_reader.h"

class VIOPipeline {
private:
    std::unique_ptr<CameraReader> camera_reader;
    std::unique_ptr<ImuReader> imu_reader;
    
public:
    VIOPipeline(
        std::unique_ptr<CameraReader> camera_reader,
        std::unique_ptr<ImuReader> imu_reader
    );

    void run();
};