#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void capture(const char* config_path, bool bin_config, int num) {
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
    for (int i = 0; i < num; i++) {
        ArducamImageFrame image;
        // the ArducamCaptureImage return Success if success, otherwise return error code
        if (ArducamCaptureImage(camera, &image, 1000) == Success) {
            printf("get frame(%dx%d) from camera.\n", image.format.width, image.format.height);
        }
    }
    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}
