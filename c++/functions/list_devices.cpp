#include <arducam/ArducamCamera.hpp>
#include <cstdlib>
#include <iostream>

void list_devices() {
    Arducam::DeviceList devs = Arducam::Camera::listDevices();
    std::cout << "Total " << devs.size() << " devices.\n";
    for (int index = 0; index < devs.size(); index++) {
        std::cout << index + 1 << ": " << devs[index] << "\n";
    }
}
