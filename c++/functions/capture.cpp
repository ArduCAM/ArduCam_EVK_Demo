#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

void capture(const char* config_path, bool bin_config, int num) {
    Arducam::Camera camera;
    Arducam::Param param;
    param.config_file_name = config_path;  // a path of config file
    param.bin_config = bin_config;         // if the config file is a bin file
    if (!camera.open(param)) {             // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }
    camera.init();  // init camera
    camera.start();
    for (int i = 0; i < num; i++) {
        Arducam::Frame image;
        // the capture return true if success, otherwise return false
        if (camera.capture(image, 1000)) {
            std::cout << "get frame(" << image.format.width << "x" << image.format.height << ") from camera.\n";
        }
    }
    camera.stop();
    camera.close();
}
