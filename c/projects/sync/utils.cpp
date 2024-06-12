#include "utils.h"

#include <map>

#define RGB565_RED   0xf800
#define RGB565_GREEN 0x07e0
#define RGB565_BLUE  0x001f

#define RAW_RG   0
#define RAW_GR   1
#define RAW_GB   2
#define RAW_BG   3
#define RAW_GRAY 4

#define LOCAL static inline

const std::map<int, int> cvt_code_map{
    {0, cv::COLOR_BayerRG2BGR}, {1, cv::COLOR_BayerGR2BGR}, {2, cv::COLOR_BayerGB2BGR},
    {3, cv::COLOR_BayerBG2BGR}, {4, cv::COLOR_GRAY2BGR},
};

LOCAL cv::Mat JPGToMat(uint8_t* bytes, int length) {
    cv::Mat image = cv::Mat(1, length, CV_8UC1, bytes);
    if (length <= 0) {
        image.data = NULL;
        return image;
    }

    image = imdecode(image, cv::IMREAD_ANYCOLOR);
    return image;
}
LOCAL cv::Mat YUV422toMat(uint8_t* bytes, int width, int height) {
    cv::Mat image = cv::Mat(height, width, CV_8UC2, bytes);
    cv::cvtColor(image, image, cv::COLOR_YUV2BGR_YUYV);
    return image;
}
LOCAL cv::Mat separationImage(uint8_t* bytes, int width, int height) {
    int width_d = width << 1;
    unsigned char *temp1, *temp2;
    temp1 = (unsigned char*)malloc(width);
    temp2 = (unsigned char*)malloc(width);

    for (int k = 0; k < height; k++) {
        for (int i = 0, j = 0; i < width_d; i += 2) {
            temp1[j] = bytes[i + (k * width_d)];
            temp2[j++] = bytes[i + 1 + (k * width_d)];
        }
        memcpy(bytes + (k * width_d), temp1, width);
        memcpy(bytes + (k * width_d + width), temp2, width);
    }
    cv::Mat image = cv::Mat(height, width_d, CV_8UC1, bytes);
    free(temp1);
    free(temp2);
    return image;
}
LOCAL cv::Mat RGB565toMat(uint8_t* bytes, int width, int height, int color_mode) {
    unsigned char *temp_data, *ptdata, *data, *data_end;

    data = bytes;
    data_end = bytes + (width * height * 2);

    temp_data = (unsigned char*)malloc(width * height * 3);
    ptdata = temp_data;

    uint8_t r, g, b;
    while (data < data_end) {
        unsigned short temp;

        temp = (*data << 8) | *(data + 1);
        r = (temp & RGB565_RED) >> 8;
        g = (temp & RGB565_GREEN) >> 3;
        b = (temp & RGB565_BLUE) << 3;

        switch (color_mode) {
        case 1:
            *ptdata++ = r;
            *ptdata++ = g;
            *ptdata++ = b;
            break;
        case 0:
        default:
            *ptdata++ = b;
            *ptdata++ = g;
            *ptdata++ = r;
            break;
        }
        data += 2;
    }

    cv::Mat image = cv::Mat(height, width, CV_8UC3);
    memcpy(image.data, temp_data, width * height * 3);
    cv::flip(image, image, 0);
    free(temp_data);
    return image;
}
LOCAL cv::Mat dBytesToMat(uint8_t* bytes, int bit_width, int width, int height) {
    unsigned char* temp_data = (unsigned char*)malloc(width * height);
    int index = 0;
    for (int i = 0; i < width * height * 2; i += 2) {
        unsigned char temp = ((bytes[i + 1] << 8 | bytes[i]) >> (bit_width - 8)) & 0xFF;
        temp_data[index++] = temp;
    }
    cv::Mat image = cv::Mat(height, width, CV_8UC1);
    memcpy(image.data, temp_data, height * width);
    free(temp_data);
    return image;
}
LOCAL cv::Mat BytestoMat(uint8_t* bytes, int width, int height) {
    cv::Mat image = cv::Mat(height, width, CV_8UC1, bytes);
    return image;
}
LOCAL void BayerToBGR(cv::Mat& data, int color_mode) {
    switch (color_mode) {
    case RAW_RG:
        cv::cvtColor(data, data, cv::COLOR_BayerRG2BGR);
        break;
    case RAW_GR:
        cv::cvtColor(data, data, cv::COLOR_BayerGR2BGR);
        break;
    case RAW_GB:
        cv::cvtColor(data, data, cv::COLOR_BayerGB2BGR);
        break;
    case RAW_BG:
        cv::cvtColor(data, data, cv::COLOR_BayerBG2BGR);
        break;
    case RAW_GRAY:
        cv::cvtColor(data, data, cv::COLOR_GRAY2BGR);
        break;
    default:
        cv::cvtColor(data, data, cv::COLOR_BayerRG2BGR);
        break;
    }
}
LOCAL cv::Mat ConvertImage(uint8_t* data, int size, int height, int width, int format, int bit_width) {
    cv::Mat rawImage;
    int fmt_mode = (format >> 8) & 0xFF;
    int color_mode = format & 0xFF;

    switch (fmt_mode) {
    case FORMAT_MODE_RGB:
        rawImage = RGB565toMat(data, width, height, color_mode);
        break;
    case FORMAT_MODE_RAW_D:
        rawImage = separationImage(data, width, height);
        BayerToBGR(rawImage, color_mode);
        break;
    case FORMAT_MODE_MON_D:
        rawImage = separationImage(data, width, height);
        break;
    case FORMAT_MODE_JPG:
        rawImage = JPGToMat(data, size);
        break;
    case FORMAT_MODE_RAW:
        if (bit_width != 8) {
            rawImage = dBytesToMat(data, bit_width, width, height);
        } else {
            rawImage = BytestoMat(data, width, height);
        }
        BayerToBGR(rawImage, color_mode);
        break;
    case FORMAT_MODE_YUV:
        rawImage = YUV422toMat(data, width, height);
        break;
    case FORMAT_MODE_MON:
        if (bit_width != 8) {
            rawImage = dBytesToMat(data, bit_width, width, height);
        } else {
            rawImage = BytestoMat(data, width, height);
        }
        break;
    default:
        if (bit_width != 8) {
            rawImage = dBytesToMat(data, bit_width, width, height);
        } else {
            rawImage = BytestoMat(data, width, height);
        }
        cv::cvtColor(rawImage, rawImage, cv::COLOR_BayerRG2RGB);
        break;
    }
    return rawImage;
}

cv::Mat from_image(ArducamImageFrame image) {
    cv::Mat color_frame = ConvertImage(image.data, image.size, image.format.height, image.format.width,
                                       image.format.format, image.format.bit_width);

    // if color_frame width > 720, resize it to 720
    if (color_frame.cols > 720) {
        auto width = color_frame.cols;
        auto scale = 720.0 / width;
        cv::resize(color_frame, color_frame, cv::Size(), scale, scale);
    }

    return color_frame;
}

void show_image(ArducamImageFrame image, const char* id) {
    cv::imshow(id, from_image(image));
    cv::setWindowTitle(id, "Test " + std::to_string(image.seq));
}
