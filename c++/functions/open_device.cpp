#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

void open_device(const char* config_path, bool bin_config, int device_index) {
    Arducam::Camera camera;
    Arducam::Param param;
    // get all devices
    Arducam::DeviceList devs = Arducam::Camera::listDevices();
    // make the index start from 0
    device_index -= 1;

    if (device_index < 0) {
        std::cout << "device index must be greater than 1.\n";
        std::exit(-1);
    }

    if (device_index >= devs.size()) {
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
