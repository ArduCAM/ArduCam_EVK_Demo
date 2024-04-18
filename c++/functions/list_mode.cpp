#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

void list_mode(const char* config_path, int mode_id, bool only_list) {
    Arducam::Camera camera;
    Arducam::Param param;
    param.config_file_name = config_path;  // a path of config file
    param.bin_config = true;               // list mode only support bin config
    if (!camera.open(param)) {             // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }
    if (only_list) {  // get the bin config
        auto configs_size = camera.modeSize();
        std::vector<ArducamCameraConfig> configs(configs_size);
        std::vector<uint32_t> config_ids(configs_size);
        camera.listMode(config_ids.data(), configs.data());
        // print sensor info
        std::cout << "Sensor info: \n";
        for (uint32_t i = 0; i < configs_size; i++) {
            const auto& id_ = config_ids[i];
            const auto& info = configs[i];
            std::cout << id_ << ": " << info.width << "x" << info.height << "\n";
        }
    } else {
        camera.init();  // init camera
        // print current resolution
        auto config = camera.config();
        std::cout << "Current resolution: " << config.width << "x" << config.height << "\n";
        // switch to sensor mode
        if (!camera.switchMode(mode_id)) {  // will return false if failed
            std::cout << "switch mode error! " << camera.lastErrorMessage() << "\n";
            std::exit(-1);
        }
        std::cout << "Switch to mode_id=" << mode_id << "\n";
        // print current resolution
        config = camera.config();
        std::cout << "Current resolution: " << config.width << "x" << config.height << "\n";

        camera.close();
    }
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (int, i, id, "Sensor Mode ID."),
        (lit, l, list, "List all sensor mode.")
    );
    // clang-format on
    const char* info = "List sensor mode or switch to sensor mode.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    if (!bin) {
        std::cerr << "List mode only support bin config.\n";
        return -1;
    }
    list_mode(path, GET_OR_DEFAULT(id, 0), list->count > 0);

    ARGPARSE_FREE(parse);
    return 0;
}
