#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"

void open_advanced(const char* config_path, bool bin_config, bool dma_enable) {
    ArducamCameraHandle camera;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);  // init camera open parameters with the default values

    // the config file is optional.
    // if not set, you must set the register value by yourself, after open camera and before init camera
    if (config_path != NULL) {
        param.config_file_name = config_path;  // a path of config file
        param.bin_config = bin_config;         // if the config file is a bin file
    }

    param.mem_type = dma_enable ? DMA : RAM;  // set memory type

    int ret =
        ArducamOpenCamera(&camera, &param);  // open camera, return Success if success, otherwise return error code
    if (ret != Success) {
        // get the last error message
        printf("open error! %s\n", ArducamErrorName(ret));
        exit(-1);
    }

    if (config_path == NULL) {
        ArducamCameraConfig config;
        config.height = 1080;                     // set height (camera, required)
        config.width = 1920;                      // set width (camera, required)
        ArducamSetCameraConfig(camera, &config);  // set config to camera
        // set register value by yourself
    }

    ArducamInitCamera(camera);  // init camera
    // ...
    ArducamCloseCamera(camera);
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (lit, d, dma, "Enable DMA.")
    );
    // clang-format on
    const char* info = "Open camera with advanced options.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    open_advanced(path, bin, dma->count > 0);

    ARGPARSE_FREE(parse);
    return 0;
}
