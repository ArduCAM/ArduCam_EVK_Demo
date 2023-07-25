#pragma once

#include <opencv2/opencv.hpp>

#include "arducam/ArducamCamera.hpp"

cv::Mat from_image(ArducamImageFrame image);
void show_image(ArducamImageFrame image);
