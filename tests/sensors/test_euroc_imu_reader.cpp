#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "sensors/euroc_imu_reader.h"

namespace fs = std::filesystem;

class EurocImuReaderTest : public ::testing::Test {
protected:
    fs::path tmp_root;
    int mock_data_length = 50;

    void SetUp() override {
        tmp_root = fs::temp_directory_path() / "vio_imu_test";
        fs::create_directories(tmp_root / "mav0/imu0");
        write_csv();
    }

    void TearDown() override {
        fs::remove_all(tmp_root);
    }

private:
    void write_csv() {
        std::ofstream imu_csv(tmp_root / "mav0/imu0/data.csv");

        const std::string mock_sample = "0.0";

        imu_csv << "#timestamp [ns]," << "w_RS_S_x [rad s^-1]," 
            << "w_RS_S_y [rad s^-1]," << "w_RS_S_z [rad s^-1],"
            << "a_RS_S_x [m s^-2]," << "a_RS_S_y [m s^-2],"
            << "a_RS_S_z [m s^-2]\n";
        
        for(int i = 0; i < mock_data_length; i++){
            imu_csv << std::to_string(i) << "," << mock_sample << "," << mock_sample << ","
                << mock_sample << "," << mock_sample << "," << mock_sample << ","
                << mock_sample << "\n";
        }
    }
};

TEST_F(EurocImuReaderTest, ReadsAllSamples) {
    EurocImuReader imu_reader(tmp_root.string());

    int samples_read = 0;
    while(imu_reader.is_open()){
        auto imu_sample = imu_reader.read_next();

        ASSERT_TRUE(imu_sample.has_value());
        EXPECT_DOUBLE_EQ(imu_sample->gyro_x, 0.0);
        EXPECT_DOUBLE_EQ(imu_sample->accel_x, 0.0);
        EXPECT_EQ(imu_sample->timestamp, samples_read);

        samples_read++;
    }

    EXPECT_EQ(samples_read, mock_data_length);
}

TEST_F(EurocImuReaderTest, ReturnsNulloptAfterExhaustion) {
    EurocImuReader imu_reader(tmp_root.string());

    while(imu_reader.is_open()){
        imu_reader.read_next();
    }

    EXPECT_EQ(imu_reader.read_next(), std::nullopt);
}

TEST(EurocImuReader, HandlesMissingDataset) {
    testing::internal::CaptureStderr();
    EurocImuReader imu_reader("/invalid/dataset/path");
    std::string stderr_output = testing::internal::GetCapturedStderr();

    EXPECT_FALSE(imu_reader.is_open());
    EXPECT_EQ(imu_reader.read_next(), std::nullopt);
    EXPECT_NE(stderr_output.find("/invalid/dataset/path"), std::string::npos);
}