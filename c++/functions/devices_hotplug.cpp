#include <arducam/ArducamCamera.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "options.h"

using namespace std::literals;

static std::string to_name(const uint8_t serial_number[16]) {
    std::string arr = "xxxx-xxxx-xxxx";
    // xxxx-xxxx-xxxx
    for (int i = 0; i < 4; i++) {
        arr[i] = isprint(serial_number[i]) ? serial_number[i] : '?';
    }
    for (int i = 5; i < 9; i++) {
        arr[i] = isprint(serial_number[i - 1]) ? serial_number[i - 1] : '?';
    }
    for (int i = 10; i < 14; i++) {
        arr[i] = isprint(serial_number[i - 2]) ? serial_number[i - 2] : '?';
    }
    return arr;
}

static void callback(ArducamEventCode event, ArducamDeviceHandle device) {
    switch (event) {
    case DeviceConnect:
        std::cout << "New device[" << device << "] connected\n";  // this is a null pointer
        break;
    case DeviceDisconnect:
        std::cout << "Device[" << device << "][" << to_name(device->serial_number) << "] disconnected\n";
        break;
    default:
        break;
    }
}

void device_hotplug(double delay) {
    Arducam::DeviceList devs = Arducam::DeviceList::listDevices();
    devs.setEventCallback(callback);
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 1000)));
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
