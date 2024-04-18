#include <arducam/ArducamCamera.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "options.h"

static void callback(Arducam::Frame image) {
    std::cout << "get frame(" << image.format.width << "x" << image.format.height << ") from camera.\n";
}

void capture_async(const char* config_path, bool bin_config, double delay) {
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

    // set read callback, this should before camera.start()
    camera.setCaptureCallback(callback);

    camera.start();
    // use sleep_for to wait for `delay` seconds
    std::this_thread::sleep_for(std::chrono::duration<double>(delay));
    camera.stop();
    camera.close();
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (dbl, d, delay, "Delay time in seconds.")
    );
    // clang-format on
    const char* info = "Capture images asynchronously.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);
    double delay_val = GET_OR_DEFAULT(delay, 1.0);

    capture_async(path, bin, delay_val);

    ARGPARSE_FREE(parse);
    return 0;
}
