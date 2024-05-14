#include <stdbool.h>
#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void show_fps(const char* config_path, bool bin_config) {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);           // init camera open parameters with the default values
    param.config_file_name = config_path;  // a path of config file
    param.bin_config = bin_config;         // if the config file is a bin file

    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }

    ArducamInitCamera(camera);  // init camera
    ArducamStartCamera(camera);

    // wait 2 seconds
    sleep(2);

    // show fps and bandwidth
    printf("fps: %f, bandwidth: %d B/s (%.2f MB/s)\n",  //
           ArducamGetCaptureFps(camera),                //
           ArducamGetBandwidth(camera),                 //
           ArducamGetBandwidth(camera) / 1024. / 1024);
    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}
