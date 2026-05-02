cmake_minimum_required(VERSION 3.30)
project(vio_pipeline)

option(VIO_BUILD_TESTING "Build VIO tests" ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POLICY_DEFAULT_CMP0167 NEW)

find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
find_package(OpenCV REQUIRED)
find_package(GTSAM REQUIRED)
find_package(Eigen3 REQUIRED)

# Component libraries

add_library(vio_types
    src/types/config.cpp
)
target_include_directories(vio_types PUBLIC src)
target_link_libraries(vio_types PUBLIC ${OpenCV_LIBS})

add_library(vio_sensors
    src/sensors/euroc_camera_reader.cpp
    src/sensors/euroc_imu_reader.cpp
)
target_include_directories(vio_sensors PUBLIC src)
target_link_libraries(vio_sensors PUBLIC vio_types ${OpenCV_LIBS})

add_library(vio_frontend
    src/frontend/frontend.cpp
    src/frontend/detection/detector_factory.cpp
    src/frontend/detection/fast_detector.cpp
)
target_include_directories(vio_frontend PUBLIC src)
target_link_libraries(vio_frontend PUBLIC vio_types ${OpenCV_LIBS})

add_library(vio_pipeline_lib
    src/pipeline/vio_pipeline.cpp
)
target_include_directories(vio_pipeline_lib PUBLIC src)
target_link_libraries(vio_pipeline_lib PUBLIC vio_sensors vio_frontend)

# Main executable

add_executable(vio_pipeline src/main.cpp)
target_link_libraries(vio_pipeline PRIVATE
    vio_pipeline_lib
    Boost::program_options
)

# Tests

if(VIO_BUILD_TESTING)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()
