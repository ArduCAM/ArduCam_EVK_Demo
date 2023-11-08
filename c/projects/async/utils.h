#pragma once

#include <opencv2/opencv.hpp>

#include "arducam/arducam_evk_sdk.h"

cv::Mat from_image(ArducamImageFrame image);
void show_image(ArducamImageFrame image, const char *id);
