#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "sensors/euroc_camera_reader.h"

namespace fs = std::filesystem;

class EurocCameraReaderTest : public ::testing::Test {
protected:
    fs::path tmp_root;
    int mock_data_length = 5;

    void SetUp() override {
        tmp_root = fs::temp_directory_path() / "vio_camera_test";
        fs::create_directories(tmp_root / "mav0/cam0/data");
        fs::create_directories(tmp_root / "mav0/cam1/data");
        write_csv_and_pngs();
    }
    
    void TearDown() override {
        fs::remove_all(tmp_root);
    }

private:
    void write_csv_and_pngs() {
        cv::Mat dummy(4, 4, CV_8UC1, cv::Scalar(0));

        auto cam0 = tmp_root / "mav0/cam0/data";
        auto cam1 = tmp_root / "mav0/cam1/data";

        std::ofstream cam0_csv(tmp_root / "mav0/cam0/data.csv");
        std::ofstream cam1_csv(tmp_root / "mav0/cam1/data.csv");

        cam0_csv << "#timestamp [ns],filename\n";
        cam1_csv << "#timestamp [ns],filename\n";

        for(int i = 0; i < mock_data_length; i++){
            auto image_name = std::to_string(i);
            auto image_filename = image_name + ".png";

            cv::imwrite((cam0 / image_filename).string(), dummy);
            cv::imwrite((cam1 / image_filename).string(), dummy);

            cam0_csv << image_name << "," << image_filename << "\n";
            cam1_csv << image_name << "," << image_filename << "\n";
        }
    }
};

TEST_F(EurocCameraReaderTest, ReadsAllFrames) {
    EurocCameraReader camera_reader(tmp_root.string());
    camera_reader.open();

    int frames_read = 0;
    while(camera_reader.is_open()){
        auto frame = camera_reader.read_next();

        ASSERT_TRUE(frame.has_value());
        EXPECT_EQ(frame->timestamp, frames_read);
        EXPECT_FALSE(frame->left_image.empty());
        EXPECT_FALSE(frame->right_image.empty());

        ++frames_read;
    }

    EXPECT_EQ(frames_read, mock_data_length);
}

TEST_F(EurocCameraReaderTest, ReturnsNulloptAfterExhaustion) {
    EurocCameraReader camera_reader(tmp_root.string());
    camera_reader.open();

    while(camera_reader.is_open()){
        camera_reader.read_next();
    }

    EXPECT_EQ(camera_reader.read_next(), std::nullopt);
}

TEST_F(EurocCameraReaderTest, ThrowsOnMalformedTimestamp) {
    {
        std::ofstream cam0_csv(tmp_root / "mav0/cam0/data.csv", std::ios::app);
        std::ofstream cam1_csv(tmp_root / "mav0/cam1/data.csv", std::ios::app);
        cam0_csv << "not_timestamp,0.png\n";
        cam1_csv << "0,0.png\n";
    }

    EurocCameraReader camera_reader(tmp_root.string());
    EXPECT_THROW(camera_reader.open(), std::runtime_error);
}

TEST_F(EurocCameraReaderTest, ThrowsOnRowCountMismatch) {
    {
        std::ofstream cam0_csv(tmp_root / "mav0/cam0/data.csv", std::ios::app);
        cam0_csv << "999,extra.png";
    }

    EurocCameraReader camera_reader(tmp_root.string());
    EXPECT_THROW(camera_reader.open(), std::runtime_error);
}

TEST_F(EurocCameraReaderTest, ThrowsOnMissingImage) {
    fs::remove(tmp_root / "mav0/cam0/data/1.png");

    EurocCameraReader camera_reader(tmp_root.string());
    camera_reader.open();

    camera_reader.read_next();
    EXPECT_THROW(camera_reader.read_next(), std::runtime_error);
}

TEST(EurocCameraReader, ThrowsOnMissingDataset) {
    EurocCameraReader camera_reader("/invalid/dataset/path");
    EXPECT_THROW(camera_reader.open(), std::runtime_error);
}