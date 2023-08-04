#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

void list_mode(const char *config_path, int mode_id, bool only_list) {
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
        auto configs = camera.listMode();
        // print sensor info
        std::cout << "Sensor info: \n";
        for (auto &&config : configs) {
            // config is a pair of mode_id and sensor info, if using C++17, you can use structured binding
            // auto &&[id_, info] = config;
            auto id_ = config.first;
            auto info = config.second;
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
