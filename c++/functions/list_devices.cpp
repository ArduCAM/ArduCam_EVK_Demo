#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

#include "options.h"

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

void list_devices() {
    Arducam::DeviceList devs = Arducam::DeviceList::listDevices();
    std::cout << "Total " << devs.size() << " devices.\n";
    for (size_t index = 0; index < devs.size(); index++) {
        std::cout << index + 1 << ": " << devs[index] << " " << to_name(devs[index]->serial_number) << "\n";
    }
}

int main(int argc, char** argv) {
    list_devices();
    return 0;
}
