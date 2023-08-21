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

    // show all controls
    const Control* controls = NULL;
    uint32_t control_num = 0;
    ArducamListCtrls(camera, &controls, &control_num);
    for (uint32_t i = 0; i < control_num; i++) {
        printf("%s(%s): range(%d:%d:%d), default=%d\n", controls[i].name, controls[i].func, controls[i].min,
               controls[i].max, controls[i].step, controls[i].def);
    }
    ArducamSetCtrl(camera, "setFramerate", 1);  // set framerate to 1 fps

    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}
