# VIO Pipeline

A Visual-Inertial Odometry pipeline targeting the [EuRoC MAV dataset](https://projects.asl.ethz.ch/datasets/euroc-mav/)

**Status:** frontend, motion estimator, and factor graph backend are being actively developed.

## Dependencies

- CMake ≥ 3.30, C++17 toolchain
- Eigen3, OpenCV 4, Boost (program_options), TBB, SuiteSparse
- [GTSAM 4.2](https://github.com/borglab/gtsam) (built from source)
- GoogleTest

## Setup

On Ubuntu 22.04, run the installation script from the repo root:

```bash
./scripts/install_deps.sh
```

This installs CMake 3.30, system packages, builds GTSAM 4.2 from source, installs analysis tools, and downloads the EuRoC MH_01 dataset into `data/` when missing.

## Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Run

```bash
./vio_pipeline -c ../configs/configs.yaml
```

The config points the pipeline at the EuRoC MAV dataset root, initially set to `../data/machine_hall/MH_01_easy`.

## Tests

```bash
ctest --output-on-failure
```

Set `-DVIO_BUILD_TESTING=OFF` when running `cmake` if you only want the executable.

## Project layout

```
src/
├── frontend/   detection/tracking interfaces
├── pipeline/   top-level orchestration
├── sensors/    camera/IMU interfaces
├── types/      data types
└── utils/      utilities

tests/          unit tests
configs/        configuration files
```

## License

Apache License 2.0 — see [LICENSE](LICENSE).