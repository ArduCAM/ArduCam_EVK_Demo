#include <arducam/arducam_evk_sdk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"

#define USB_CPLD_I2C_ADDRESS 0x46

static void dump_info(ArducamCameraHandle handle) {
    uint32_t version, year, mouth, day;

#define read_8_8(reg, val)                                                        \
    if (Success != ArducamReadReg_8_8(handle, USB_CPLD_I2C_ADDRESS, reg, &val)) { \
        fprintf(stderr, "Failed to read " #val "\n");                             \
        return;                                                                   \
    }

    read_8_8(0x00, version);
    read_8_8(0x05, year);
    read_8_8(0x06, mouth);
    read_8_8(0x07, day);

#undef read_8_8

    printf("version: v%d.%d\n", version >> 4, version & 0x0F);
    printf("year: %d\n", year);
    printf("mouth: %d\n", mouth);
    printf("day: %d\n", day);

    uint8_t buf[4];
    ArducamReadBoardConfig(handle, VR_DEVICE_TO_HOST, 0x00, 0x00, 2, buf);
    printf("fw_version: v%d.%d\n", (buf[0] & 0xFF), (buf[1] & 0xFF));
    printf("usb: %s\n", ArducamGetUSBType(handle));
}

void open_basic(const char* config_path, bool bin_config) {
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
    dump_info(camera);
    ArducamCloseCamera(camera);
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (file, c, config, "Path to config file.")
    );
    // clang-format on
    const char* info = "Dump camera information.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);
    CHECK_REQUIRED(config, return 1);

    GET_CONFIG(config, path, bin);

    open_basic(path, bin);

    ARGPARSE_FREE(parse);
    return 0;
}
