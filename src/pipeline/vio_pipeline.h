#pragma once

#include <memory>
#include "sensors/camera_reader.h"
#include "sensors/imu_reader.h"
#include "frontend/frontend.h"

class VIOPipeline {
private:
    std::unique_ptr<CameraReader> camera_reader_;
    std::unique_ptr<ImuReader> imu_reader_;
    Frontend frontend_;
    
public:
    VIOPipeline(
        std::unique_ptr<CameraReader> camera_reader,
        std::unique_ptr<ImuReader> imu_reader,
        Frontend frontend
    );

    void run();
};

VIOPipeline make_pipeline(const Config& config);