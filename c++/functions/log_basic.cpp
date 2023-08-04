#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

void log_basic() {
    Arducam::Camera camera;
    Arducam::Param param;                // use a default parameter
    if (!camera.open(param)) {  // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }
    // get current log level
    std::cout << "Current Log Level: " << camera.logLevel() << "\n";
    std::cout << "Set the Log Level to Trace\n";
    // set log level to info
    camera.setLogLevel(Arducam::LoggerLevel::trace);
    // get current log level
    std::cout << "Current Log Level: " << camera.logLevel() << "\n";

    // set camera config
    Arducam::CameraConfig camera_config;
    camera_config.height = 1080;      // set height (required)
    camera_config.width = 1920;       // set width (required)
    camera.setConfig(camera_config);  // set config to camera

    // init camera
    camera.init();

    std::cout << "Disable Console Log\n";
    camera.enableConsoleLog(false);  // disable console log
    camera.close();
}
