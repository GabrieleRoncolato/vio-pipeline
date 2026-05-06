#include "pipeline/vio_pipeline.h"
#include "sensors/euroc_camera_reader.h"
#include "sensors/euroc_imu_reader.h"
#include "frontend/detection/detector_factory.h"
#include "frontend/selection/selector_factory.h"

VIOPipeline::VIOPipeline(
    std::unique_ptr<CameraReader> camera_reader,
    std::unique_ptr<ImuReader> imu_reader,
    Frontend frontend
) : 
    camera_reader_(std::move(camera_reader)),
    imu_reader_(std::move(imu_reader)),
    frontend_(std::move(frontend))
{}

void VIOPipeline::run() {
    camera_reader_->open();
    imu_reader_->open();

    while(true){
        auto frame_opt = camera_reader_->read_next();
        if(!frame_opt) break;

        frontend_.process(*frame_opt);
    }
}

VIOPipeline make_pipeline(const Config& config) {
    return VIOPipeline(
        std::make_unique<EurocCameraReader>(config.dataset_path),
        std::make_unique<EurocImuReader>(config.dataset_path),
        Frontend(
            make_detector(config.frontend_config.detector_config),
            make_selector(config.frontend_config.selector_config)
        )
    );
}