#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"

static const char *to_name(ArducamLoggerLevel level) {
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

void log_(ArducamLoggerLevel level, const char *msg, void *user_data) {
    // log callback
    printf("[%s] %s\n", to_name(level), msg);
}

void log_callback() {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);  // init camera open parameters with the default values
    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }
    printf("Disable Console Log\n");
    ArducamDisableConsoleLog(camera);                    // disable console log
    ArducamSetLogLevel(camera, trace);                   // set log level to info
    ArducamRegisterMessageCallback(camera, log_, NULL);  // set log callback

    // set camera config
    ArducamCameraConfig config;
    config.height = 1080;                     // set height (camera, required)
    config.width = 1920;                      // set width (camera, required)
    ArducamSetCameraConfig(camera, &config);  // set config to camera

    // init camera
    ArducamInitCamera(camera);
    ArducamCloseCamera(camera);
}

int main(int argc, char **argv) {
    log_callback();
    return 0;
}
