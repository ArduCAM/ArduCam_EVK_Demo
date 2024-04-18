#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

void set_transfer_option(const char* config_path, bool bin_config, int count, int size, int num) {
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

    // set transfer option, this should be done before camera.start()
    camera.setTransfer(count, size);

    camera.start();
    for (int i = 0; i < num; i++) {
        // the capture should return a Frame object or nullptr
        Arducam::Frame frame;
        if (camera.capture(frame, 1000)) {
            std::cout << "get frame(" << frame.format.width << "x" << frame.format.height << ") from camera."
                      << "\n";
            camera.freeImage(frame);
        }
    }
    camera.stop();
    camera.close();
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (int, t, count, "Transfer count."),
        (int, s, size, "Transfer size."),
        (int, n, take, "Number of frames to take.")
    );
    // clang-format on
    const char* info = "Set transfer option.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);
    int count_val = GET_OR_DEFAULT(count, 30);
    int size_val = GET_OR_DEFAULT(size, 512 * 1024);
    int take_val = GET_OR_DEFAULT(take, 1);

    set_transfer_option(path, bin, count_val, size_val, take_val);

    ARGPARSE_FREE(parse);
    return 0;
}
