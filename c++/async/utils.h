#pragma once

#include <opencv2/opencv.hpp>

#include "arducam/ArducamCamera.hpp"

cv::Mat from_buffer(ArducamFrameBuffer fb);
void show_buffer(ArducamFrameBuffer fb);
