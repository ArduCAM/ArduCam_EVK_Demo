#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"

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
            const ArducamCameraConfig *info = configs + i;
            const uint32_t *id_ = ids + i;
            printf("%d: %dx%d\n", *id_, info->width, info->height);
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

int main(int argc, char **argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (int, i, id, "Sensor Mode ID."),
        (lit, l, list, "List all sensor mode.")
    );
    // clang-format on
    const char* info = "List sensor mode or switch to sensor mode.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    if (!bin) {
        fprintf(stderr, "List mode only support bin config.\n");
        return -1;
    }
    list_mode(path, GET_OR_DEFAULT(id, 0), list->count > 0);

    ARGPARSE_FREE(parse);
    return 0;
}
