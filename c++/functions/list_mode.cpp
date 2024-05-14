#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

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
