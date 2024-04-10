#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>

const char* to_name(char arr[15], const uint8_t serial_number[16]) {
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

void list_devices() {
    ArducamDeviceListHandle devs;
    ArducamListDevice(&devs);
    printf("Total %d devices.\n", devs->size);
    for (int index = 0; index < devs->size; index++) {
        ArducamDeviceHandle d = devs->devices[index];
        printf("%d: [%04x:%04x] %s\n", index + 1, d->id_vendor, d->id_product, to_name((char[16]){}, d->serial_number));
    }
}
