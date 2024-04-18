#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

static const char* to_name(Arducam::LoggerLevel level) {
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
    return "unknown";
}

void log_(Arducam::LoggerLevel level, const char* msg, int size) {
    // log callback
    std::cout << "[" << to_name(level) << "] " << std::string(msg, size) << "\n";
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
    camera.enableConsoleLog(false);                   // disable console log
    camera.setLogLevel(Arducam::LoggerLevel::trace);  // set log level to info
    camera.setMessageCallback(log_);                  // set log callback

    // set camera config
    Arducam::CameraConfig camera_config;
    camera_config.height = 1080;      // set height (required)
    camera_config.width = 1920;       // set width (required)
    camera.setConfig(camera_config);  // set config to camera

    // init camera
    camera.init();
    camera.close();
}

int main(int argc, char** argv) {
    log_callback();
    return 0;
}
