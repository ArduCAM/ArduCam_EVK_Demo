#include "utils.h"

#include <map>

const std::map<int, int> cvt_code_map{
    {0, cv::COLOR_BayerRG2BGR}, {1, cv::COLOR_BayerGR2BGR}, {2, cv::COLOR_BayerGB2BGR},
    {3, cv::COLOR_BayerBG2BGR}, {4, cv::COLOR_GRAY2BGR},
};

cv::Mat from_image(ArducamImageFrame image) {
    cv::Mat frame;
    uint32_t resolution = image.format.width * image.format.height;

    if (image.format.bit_width == 8) {
        frame = cv::Mat(image.format.height, image.format.width, CV_8UC1, image.data);
    } else {
        uint16_t *data = reinterpret_cast<uint16_t *>(image.data);
        const int l_shift = 16 - image.format.bit_width;
        for (uint32_t i = 0; i < resolution; i++) {
            data[i] <<= l_shift;
        }
        frame = cv::Mat(image.format.height, image.format.width, CV_16UC1, image.data);
    }

    cv::Mat color_frame;
    cv::cvtColor(frame, color_frame, cvt_code_map.at(image.format.format & 0xff));

    // if color_frame width > 720, resize it to 720
    if (color_frame.cols > 720) {
        auto width = color_frame.cols;
        auto scale = 720.0 / width;
        cv::resize(color_frame, color_frame, cv::Size(), scale, scale);
    }

    return color_frame;
}

void show_image(ArducamImageFrame image) {
    cv::imshow("Test", from_image(image));
    cv::setWindowTitle("Test", "Test " + std::to_string(image.seq));
}
