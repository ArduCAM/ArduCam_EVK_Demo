#include <arducam/ArducamCamera.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::literals;

std::string to_name(const uint8_t serial_number[16]) {
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

void callback(ArducamEventCode event, ArducamDeviceHandle device) {
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

void device_hotplug() {
    Arducam::DeviceList devs = Arducam::DeviceList::listDevices();
    devs.setEventCallback(callback);
    std::this_thread::sleep_for(10s);
}
