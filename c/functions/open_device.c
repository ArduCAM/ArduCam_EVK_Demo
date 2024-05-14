#include <stdbool.h>
#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>

void open_device(const char* config_path, bool bin_config, int device_index) {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);  // init camera open parameters with the default values
    // get all devices
    ArducamDeviceListHandle devs;
    ArducamListDevice(&devs);
    // make the index start from 0
    device_index -= 1;

    if (device_index < 0) {
        printf("device index must be greater than 1.\n");
        exit(-1);
    }

    if (device_index >= devs->size) {
        printf("device index out of range, total %d devices.\n", devs->size);
        exit(-1);
    }

    param.config_file_name = config_path;        // a path of config file
    param.bin_config = bin_config;               // if the config file is a bin file
    param.device = devs->devices[device_index];  // set the opened device

    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }

    ArducamInitCamera(camera);  // init camera
    // ...
    ArducamCloseCamera(camera);
}
