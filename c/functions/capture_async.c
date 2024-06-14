#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#endif

#include "options.h"

static void delay_ms(int mills) {
#ifdef __linux__
    usleep(mills * 1000);
#endif
#ifdef _WIN32
    Sleep(mills);
#endif
}

static void callback(ArducamImageFrame image, void* user_data) {
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
    delay_ms((int)(delay * 1000));

    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file."),
        (dbl, d, delay, "Delay time in seconds.")
    );
    // clang-format on
    const char* info = "Capture images asynchronously.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);
    double delay_val = GET_OR_DEFAULT(dbl, delay, 1.0);

    capture_async(path, bin, delay_val);

    ARGPARSE_FREE(parse);
    return 0;
}
