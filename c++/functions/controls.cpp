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

    // show all controls
    for (const Control& control : camera.controls()) {
        std::cout << control.name << "(" << control.func << "): range(" << control.min << ":" << control.max << ":"
                  << control.step << "), default=" << control.def << "\n";
    }
    camera.setControl("setFramerate", 1);  // set framerate to 1 fps

    camera.stop();
    camera.close();
}
