#include <arducam/arducam_evk_sdk.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "options.h"

static const char* to_name(char arr[15], const uint8_t serial_number[16]) {
    // xxxx-xxxx-xxxx
    for (int i = 0; i < 4; i++) {
        arr[i] = isprint(serial_number[i]) ? serial_number[i] : '?';
    }
    arr[4] = '-';
    for (int i = 5; i < 9; i++) {
        arr[i] = isprint(serial_number[i - 1]) ? serial_number[i - 1] : '?';
    }
    arr[9] = '-';
    for (int i = 10; i < 14; i++) {
        arr[i] = isprint(serial_number[i - 2]) ? serial_number[i - 2] : '?';
    }
    arr[14] = '\0';
    return arr;
}

static void callback(ArducamEventCode event, ArducamDeviceHandle device, void* user_data) {
    switch (event) {
    case DeviceConnect:
        assert(device == NULL);
        printf("New device[%p] connected\n", device);  // this is a null pointer
        break;
    case DeviceDisconnect:
        assert(device != NULL);
        printf("Device[%p][%s] disconnected\n", device, to_name((char[16]){}, device->serial_number));
        break;
    }
}

void device_hotplug(double delay) {
    ArducamDeviceListHandle devs;
    ArducamListDevice(&devs);
    ArducamDeviceListRegisterEventCallback(devs, callback, NULL);
    usleep(delay * 1000 * 1000);
    ArducamFreeDeviceList();
}

int main(int argc, char** argv) {
    // clang-format off
    ARGPARSE_DEFINE(parse,
        (dbl, d, delay, "Delay time in seconds.")
    );
    // clang-format on
    const char* info = "Monitor device hotplug events.";
    ARGPARSE_PARSE(parse, argc, argv, info, return 1, return 0);

    double delay_val = GET_OR_DEFAULT(delay, 10.0);

    device_hotplug(delay_val);

    ARGPARSE_FREE(parse);
    return 0;
}
