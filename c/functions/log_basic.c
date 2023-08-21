#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>

const char* to_name(ArducamLoggerLevel level) {
    switch (level) {
        case trace:
            return "trace";
        case debug:
            return "debug";
        case info:
            return "info";
        case warn:
            return "warn";
        case err:
            return "err";
        case critical:
            return "critical";
        case off:
            return "off";
    }
}

void log_basic() {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;  // use a default parameter
    ArducamDefaultParam(&param);   // init camera open parameters with the default values
    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }
    ArducamLoggerLevel level;
    // get current log level
    ArducamGetLogLevel(camera, &level);
    printf("Current Log Level: %s\n", to_name(level));
    printf("Set the Log Level to Trace\n");
    // set log level to info
    ArducamSetLogLevel(camera, trace);
    // get current log level
    ArducamGetLogLevel(camera, &level);
    printf("Current Log Level: %s\n", to_name(level));

    // set camera config
    ArducamCameraConfig config;
    config.height = 1080;                     // set height (camera, required)
    config.width = 1920;                      // set width (camera, required)
    ArducamSetCameraConfig(camera, &config);  // set config to camera

    // init camera
    ArducamInitCamera(camera);

    printf("Disable Console Log\n");
    ArducamDisableConsoleLog(camera);  // disable console log
    ArducamCloseCamera(camera);
}
