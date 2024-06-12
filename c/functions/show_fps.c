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
    delay_ms(2000);

    // show fps and bandwidth
    printf("fps: %d, bandwidth: %d B/s (%.2f MB/s)\n",  //
           ArducamGetCaptureFps(camera),                //
           ArducamGetBandwidth(camera),                 //
           ArducamGetBandwidth(camera) / 1024. / 1024);
    ArducamStopCamera(camera);
    ArducamCloseCamera(camera);
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file.")
    );
    // clang-format on
    const char* info = "Show FPS and bandwidth.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    show_fps(path, bin);

    ARGPARSE_FREE(parse);
    return 0;
}
