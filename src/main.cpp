#include <boost/program_options.hpp>
#include <iostream>
#include <memory>

#include "types/config.h"
#include "pipeline/vio_pipeline.h"
#include "sensors/euroc_camera_reader.h"
#include "sensors/euroc_imu_reader.h"

namespace po = boost::program_options;

int main(int argc, char** argv){
    po::options_description desc("VIO Pipeline");
    desc.add_options()
        ("help,h", "print help")
        ("config,c", po::value<std::string>()->default_value("../configs/configs.yaml", "Config file"));
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    po::notify(vm);

    std::string config_path = vm["config"].as<std::string>();

    Config config = Config::fromYaml(config_path);

    auto camera_reader = std::make_unique<EurocCameraReader>(config.dataset_path);
    auto imu_reader = std::make_unique<EurocImuReader>(config.dataset_path);

    VIOPipeline pipeline(
        std::move(camera_reader),
        std::move(imu_reader)
    );

    pipeline.run();

    return 0;
}