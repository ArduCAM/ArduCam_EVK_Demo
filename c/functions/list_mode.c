#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void list_mode(const char *config_path, int mode_id, bool only_list) {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);           // init camera open parameters with the default values
    param.config_file_name = config_path;  // a path of config file
    param.bin_config = true;               // list mode only support bin config
    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }
    if (only_list) {  // get the bin config
        ArducamCameraConfig *configs;
        uint32_t *ids;
        uint32_t size;
        ArducamListMode(camera, &configs, &ids, &size);
        // print sensor info
        printf("Sensor info: \n");
        for (int i = 0; i < size; i++) {
            auto info = configs[i];
            auto id_ = ids[i];
            printf("%d: %dx%d\n", id_, info.width, info.height);
        }
        ArducamFreeModeList(camera, configs, ids);
    } else {
        ArducamInitCamera(camera);  // init camera
        // print current resolution
        ArducamCameraConfig config;
        ArducamGetCameraConfig(camera, &config);
        printf("Current resolution: %dx%d\n", config.width, config.height);
        // switch to sensor mode
        ret = ArducamSwitchMode(camera, mode_id);  // will return Success if success, otherwise return error code
        if (ret != Success) {
            printf("switch mode error! %s\n", ArducamErrorName(ret));
            exit(-1);
        }
        printf("Switch to mode_id=%d\n", mode_id);
        // print current resolution
        ArducamGetCameraConfig(camera, &config);
        printf("Current resolution: %dx%d\n", config.width, config.height);

        ArducamCloseCamera(camera);
    }
}
