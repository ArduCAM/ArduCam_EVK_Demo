#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

#define USB_CPLD_I2C_ADDRESS 0x46

static void dump_info(Arducam::Camera& camera) {
    uint32_t version = 0, year = 0, mouth = 0, day = 0;

#define read_8_8(reg, val)                                                                \
    val = camera.readReg(Arducam::I2CMode::I2C_MODE_8_8, USB_CPLD_I2C_ADDRESS, reg); \
    if (camera.lastError() != Arducam::Errorcode::Success) {                              \
        std::cerr << "Failed to read " #val "\n";                                         \
        return;                                                                           \
    }

    read_8_8(0x00, version);
    read_8_8(0x05, year);
    read_8_8(0x06, mouth);
    read_8_8(0x07, day);

#undef read_8_8

    std::cout << "version: v" << (version >> 4) << "." << (version & 0x0F) << "\n";
    std::cout << "year: " << year << "\n";
    std::cout << "mouth: " << mouth << "\n";
    std::cout << "day: " << day << "\n";

    uint8_t buf[4];
    camera.readBoardConfig(VR_DEVICE_TO_HOST, 0x00, 0x00, 2, buf);
    std::cout << "fw_version: v" << (buf[0] & 0xFF) << "." << (buf[1] & 0xFF) << "\n";
    std::cout << "usb: " << camera.usbType() << "\n";
}

void open_basic(const char* config_path, bool bin_config) {
    Arducam::Camera camera;
    Arducam::Param param;
    param.config_file_name = config_path;  // a path of config file
    param.bin_config = bin_config;         // if the config file is a bin file

    if (!camera.open(param)) {  // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }

    camera.init();  // init camera
    dump_info(camera);
    camera.close();
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file.")
    );
    // clang-format on
    const char* info = "Dump camera information.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    open_basic(path, bin);

    ARGPARSE_FREE(parse);
    return 0;
}
