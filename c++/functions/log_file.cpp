#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>

void log_file() {
    Arducam::Camera camera;
    Arducam::Param param;
    std::string log_file = "log.txt";
    if (!camera.open(param)) {  // open camera, return True if success, otherwise return False
        // get the last error message
        std::cout << "open camera error! " << camera.lastErrorMessage() << "\n";
        std::exit(-1);
    }

    std::cout << "Disable Console Log\n";
    std::cout << "Save Log to " << log_file << "\n";

    camera.enableConsoleLog(false);          // disable console log
    camera.setLogLevel(Arducam::LoggerLevel::trace);  // set log level to info
    camera.addLogFile(log_file);             // add log file

    // set camera config
    Arducam::CameraConfig camera_config;
    camera_config.height = 1080;      // set height (required)
    camera_config.width = 1920;       // set width (required)
    camera.setConfig(camera_config);  // set config to camera

    // init camera
    camera.init();
    camera.close();

    std::cout << "Log File Content:\n";
    std::ifstream file(log_file);
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
}
