#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"

void set_transfer_option(const char* config_path, bool bin_config, int count, int size, int num) {
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

    // set transfer option, this should be done before ArducamStartCamera(camera)
    ArducamSetTransferConfig(camera, count, size);

    ArducamStartCamera(camera);
    for (int i = 0; i < num; i++) {
        // the ArducamCaptureImage return Success if success, otherwise return error code
        ArducamImageFrame image;
        if (ArducamCaptureImage(camera, &image, 1000) == Success) {
            printf("get frame(%dx%d) from camera.\n", image.format.width, image.format.height);
            ArducamFreeImage(camera, image);
        }
    }
    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (int, t, count, "Transfer count."),
        (int, s, size, "Transfer size."),
        (int, n, take, "Number of frames to take.")
    );
    // clang-format on
    const char* info = "Set transfer option.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);
    int count_val = GET_OR_DEFAULT(count, 30);
    int size_val = GET_OR_DEFAULT(size, 512 * 1024);
    int take_val = GET_OR_DEFAULT(take, 1);

    set_transfer_option(path, bin, count_val, size_val, take_val);

    ARGPARSE_FREE(parse);
    return 0;
}
