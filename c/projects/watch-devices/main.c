#include <arducam/arducam_evk_sdk.h>
#include <ctype.h>
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

static inline void callback(ArducamEventCode, ArducamDeviceHandle, void*);
static inline void show_list(ArducamDeviceListHandle devs);
static inline const char* to_name(char arr[15], const uint8_t serial_number[16]);

static void delay_ms(int mills) {
#ifdef __linux__
    usleep(mills * 1000);
#endif
#ifdef _WIN32
    Sleep(mills);
#endif
}

int main(int argc, char** argv) {
    ArducamDeviceListHandle devs;
    ArducamListDevice(&devs);
    show_list(devs);
    ArducamDeviceListRegisterEventCallback(devs, callback, devs);
    delay_ms(60 * 1000);
    ArducamFreeDeviceList();
    return 0;
}

static inline void callback(ArducamEventCode event, ArducamDeviceHandle device, void* user_data) {
    ArducamDeviceListHandle devs = (ArducamDeviceListHandle)user_data;
    switch (event) {
    case DeviceConnect:
        printf("\nNew device[%p] connected\n", device);  // this is a null pointer
        break;
    case DeviceDisconnect:
        printf("\nDevice[%p][%s] disconnected\n", device, to_name((char[16]){}, device->serial_number));
        break;
    }
    // must add a delay before calling ArducamRefreshDeviceList
    // if not, will show the serial number as "????-????-????"
    delay_ms(10);
    ArducamRefreshDeviceList(devs);
    show_list(devs);
}

static inline const char* to_name(char arr[15], const uint8_t serial_number[16]) {
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

static inline void show_list(ArducamDeviceListHandle devs) {
    ArducamDeviceHandle device;
    if (devs->size == 0) {
        printf("No devices found\n");
        return;
    }
    for (int i = 0; i < devs->size; i++) {
        device = devs->devices[i];
        printf("%d: Device[%p][%s]\n", i + 1, device, to_name((char[16]){}, device->serial_number));
    }
}
