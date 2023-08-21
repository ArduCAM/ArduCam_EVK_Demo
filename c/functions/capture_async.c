#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void callback(ArducamImageFrame image, void* user_data) {
    printf("get frame(%dx%d) from camera.\n", image.format.width, image.format.height);
}

void capture_async(const char* config_path, bool bin_config, double delay) {
    ArducamCameraHandle camera = NULL;
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

    // set read callback, this should before ArducamStartCamera(camera)
    ArducamRegisterCaptureCallback(camera, callback, NULL);

    ArducamStartCamera(camera);
    // use sleep_for to wait for `delay` seconds
    sleep((int)delay);
    usleep((int)((delay - (int)delay) * 1000000));

    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}
