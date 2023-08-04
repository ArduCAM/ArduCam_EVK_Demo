#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

void open_advanced(const char* config_path, bool bin_config, bool dma_enable = true) {
    Arducam::Camera camera;
    Arducam::Param param;

    // the config file is optional.
    // if not set, you must set the register value by yourself, after open camera and before init camera
    if (config_path != nullptr) {
        param.config_file_name = config_path;  // a path of config file
        param.bin_config = bin_config;         // if the config file is a bin file
    }

    param.mem_type = dma_enable ? Arducam::MemType::DMA : Arducam::MemType::RAM;  // set memory type

    if (!camera.open(param)) {  // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }

    if (config_path == nullptr) {
        Arducam::CameraConfig camera_config;
        camera_config.height = 1080;      //  set height (required)
        camera_config.width = 1920;       // set width (required)
        camera.setConfig(camera_config);  // set config to camera
        // set register value by yourself
    }

    camera.init();  // init camera
    // ...
    camera.close();
}
