include(GoogleTest)

function(add_vio_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE
        GTest::gtest_main
        ${ARGN}
    )
    target_compile_definitions(${name} PRIVATE
        FIXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures"
    )
    gtest_discover_tests(${name})
endfunction()

add_vio_test(test_config
    types/test_config.cpp
    vio_types
)
add_vio_test(test_euroc_camera_reader
    sensors/test_euroc_camera_reader.cpp
    vio_sensors
)
add_vio_test(test_euroc_imu_reader
    sensors/test_euroc_imu_reader.cpp
    vio_sensors
)
add_vio_test(test_detector_factory
    frontend/detection/test_detector_factory.cpp
    vio_frontend
)
add_vio_test(test_selector_factory
    frontend/selection/test_selector_factory.cpp
    vio_frontend
)
add_vio_test(test_bucketer_selector
    frontend/selection/test_bucketer_selector.cpp
    vio_frontend
)