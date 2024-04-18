#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"

void log_file() {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);  // init camera open parameters with the default values
    const char* log_file = "log.txt";
    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }

    printf("Disable Console Log\n");
    printf("Save Log to %s\n", log_file);

    ArducamDisableConsoleLog(camera);     // disable console log
    ArducamSetLogLevel(camera, trace);    // set log level to trace
    ArducamAddLogFile(camera, log_file);  // add log file

    // set camera config
    ArducamCameraConfig config;
    config.height = 1080;                     // set height (camera, required)
    config.width = 1920;                      // set width (camera, required)
    ArducamSetCameraConfig(camera, &config);  // set config to camera

    // init camera
    ArducamInitCamera(camera);
    ArducamCloseCamera(camera);

    printf("Log File Content:\n");
    FILE* file = fopen(log_file, "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

int main(int argc, char** argv) {
    log_file();
    return 0;
}
