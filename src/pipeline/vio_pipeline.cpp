#include "pipeline/vio_pipeline.h"
#include <iostream>

VIOPipeline::VIOPipeline(
    std::unique_ptr<CameraReader> camera_reader,
    std::unique_ptr<ImuReader> imu_reader
) : 
    camera_reader(std::move(camera_reader)),
    imu_reader(std::move(imu_reader))
{}

void VIOPipeline::run() {
    std::cout << "Running!" << "\n";
}