#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

void show_controls(const char* config_path, bool bin_config) {
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
    const Control* controls = camera.controls();
    const int size = camera.controlSize();
    for (int i = 0; i < size; i++) {
        const Control& control = controls[i];
        std::cout << control.name << "(" << control.func << "): range(" << control.min << ":" << control.max << ":"
                  << control.step << "), default=" << control.def << "\n";
    }
    camera.setControl("setFramerate", 1);  // set framerate to 1 fps

    camera.stop();
    camera.close();
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file.")
    );
    // clang-format on
    const char* info = "Show all controls.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    show_controls(path, bin);

    ARGPARSE_FREE(parse);
    return 0;
}
