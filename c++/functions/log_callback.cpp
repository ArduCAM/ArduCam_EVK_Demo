#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

const char* to_name(Arducam::LoggerLevel level) {
    switch (level) {
        case Arducam::LoggerLevel::trace:
            return "trace";
        case Arducam::LoggerLevel::debug:
            return "debug";
        case Arducam::LoggerLevel::info:
            return "info";
        case Arducam::LoggerLevel::warn:
            return "warn";
        case Arducam::LoggerLevel::err:
            return "err";
        case Arducam::LoggerLevel::critical:
            return "critical";
        case Arducam::LoggerLevel::off:
            return "off";
    }
}

void log_(Arducam::LoggerLevel level, const std::string& msg) {
    // log callback
    std::cout << "[" << to_name(level) << "] " << msg << "\n";
}

void log_callback() {
    Arducam::Camera camera;
    Arducam::Param param;
    if (!camera.open(param)) {  // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }
    std::cout << "Disable Console Log\n";
    camera.enableConsoleLog(false);          // disable console log
    camera.setLogLevel(Arducam::LoggerLevel::trace);  // set log level to info
    camera.setMessageCallback(log_);         // set log callback

    // set camera config
    Arducam::CameraConfig camera_config;
    camera_config.height = 1080;      // set height (required)
    camera_config.width = 1920;       // set width (required)
    camera.setConfig(camera_config);  // set config to camera

    // init camera
    camera.init();
    camera.close();
}
