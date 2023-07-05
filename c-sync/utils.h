#pragma once

#include <opencv2/opencv.hpp>

#include "arducam/arducam_evk_sdk.h"

cv::Mat from_buffer(ArducamFrameBuffer fb);
void show_buffer(ArducamFrameBuffer fb);
