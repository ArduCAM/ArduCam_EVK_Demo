#include <arducam/ArducamCamera.hpp>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

void show_fps(const char* config_path, bool bin_config) {
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
    camera.start();

    // wait 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // show fps and bandwidth
    std::cout << "fps: " << camera.captureFps() << ", bandwidth: " << camera.bandwidth() << "B/s (" << std::fixed
              << std::setprecision(2) << camera.bandwidth() / 1024. / 1024 << "MB/s)\n";

    camera.stop();
    camera.close();
}
