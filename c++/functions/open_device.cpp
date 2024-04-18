#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

void open_device(const char* config_path, bool bin_config, int device_index) {
    Arducam::Camera camera;
    Arducam::Param param;
    // get all devices
    Arducam::DeviceList devs = Arducam::DeviceList::listDevices();
    // make the index start from 0
    device_index -= 1;

    if (device_index < 0) {
        std::cout << "device index must be greater than 1.\n";
        std::exit(-1);
    }

    if ((size_t)device_index >= devs.size()) {
        std::cout << "device index out of range, total " << devs.size() << " devices.\n";
        std::exit(-1);
    }

    param.config_file_name = config_path;  // a path of config file
    param.bin_config = bin_config;         // if the config file is a bin file
    param.device = devs[device_index];     // set the opened device

    if (!camera.open(param)) {  // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }

    camera.init();  // init camera
    // ...
    camera.close();
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (int, d, device, "Device index. Starts from 1. Default is 1.")
    );
    // clang-format on
    const char* info = "Open camera with device index.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);
    int device_num = GET_OR_DEFAULT(device, 1);

    open_device(path, bin, device_num);

    ARGPARSE_FREE(parse);
    return 0;
}
