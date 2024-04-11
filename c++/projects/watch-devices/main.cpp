#include <arducam/ArducamCamera.hpp>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

using namespace std::literals;

static inline void show_list(Arducam::DeviceList& devs);
static inline const char* to_name(char arr[15], const uint8_t serial_number[16]);

int main(int argc, char** argv) {
    Arducam::DeviceList devs = Arducam::DeviceList::listDevices();
    show_list(devs);
    devs.setEventCallback([&devs](ArducamEventCode event, ArducamDeviceHandle device) {
        char buf[16];
        switch (event) {
        case DeviceConnect:
            printf("\nNew device[%p] connected\n", device);  // this is a null pointer
            break;
        case DeviceDisconnect:
            printf("\nDevice[%p][%s] disconnected\n", device, to_name(buf, device->serial_number));
            break;
        default:
            break;
        }
        // must add a delay before calling ArducamRefreshDeviceList
        // if not, will show the serial number as "????-????-????"
        std::this_thread::sleep_for(10ms);
        devs.refresh();
        show_list(devs);
    });
    std::this_thread::sleep_for(60s);
    return 0;
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

static inline void show_list(Arducam::DeviceList& devs) {
    if (devs.size() == 0) {
        printf("No devices found\n");
        return;
    }
    int i = 1;
    char buf[16];
    for (auto& device : devs) {
        printf("%d: Device[%p][%s]\n", i, device, to_name(buf, device->serial_number));
        i++;
    }
}
