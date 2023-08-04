#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

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
        }
    }
    camera.stop();
    camera.close();
}
