#include <stdio.h>

#include <iostream>
#include <string>
#include <string_view>

#include "utils.h"

bool exit_flag = false;

void show_image(Arducam::Camera &camera, ArducamImageFrame image) {
    int key = cv::waitKey(1);
    if (key == 'q') {
        exit_flag = true;
    }

    show_image(image, "Test");
}

void PrintDeviceInfo(Arducam::Camera &camera, ArducamDeviceHandle device) {
    printf("device id vendor: 0x%04X\n", device->id_vendor);
    printf("device id product: 0x%04X\n", device->id_product);
    printf("device in used: %d\n", device->in_used);

    const uint8_t *serial_number = device->serial_number;
    printf("device serial number: %c%c%c%c-%c%c%c%c-%c%c%c%c\n", serial_number[0], serial_number[1], serial_number[2],
           serial_number[3], serial_number[4], serial_number[5], serial_number[6], serial_number[7], serial_number[8],
           serial_number[9], serial_number[10], serial_number[11]);

    printf("device usb type: %s\n", camera.usbType().data());
    printf("device speed: %d\n", device->speed);
}

#define USB_CPLD_I2C_ADDRESS 0x46
void dumpDeviceInfo(Arducam::Camera &camera) {
    uint32_t version = 0, year = 0, mouth = 0, day = 0;
    version = camera.readReg(Arducam::I2CMode::I2C_MODE_8_8, USB_CPLD_I2C_ADDRESS, 0x00);
    year = camera.readReg(Arducam::I2CMode::I2C_MODE_8_8, USB_CPLD_I2C_ADDRESS, 0x05);
    mouth = camera.readReg(Arducam::I2CMode::I2C_MODE_8_8, USB_CPLD_I2C_ADDRESS, 0x06);
    day = camera.readReg(Arducam::I2CMode::I2C_MODE_8_8, USB_CPLD_I2C_ADDRESS, 0x07);
    printf("CPLD version: v%d.%d date: 20%d-%02d-%02d\n", version >> 4, version & 0x0F, year, mouth, day);
    Arducam::Camera::BoardConfig boardConfig = camera.readBoardConfig(0x80, 0x00, 0x00, 2);
    printf("fw_version: v%d.%d \n", boardConfig[0] & 0xFF, boardConfig[1] & 0xFF);
}

int main(int argc, char **argv) {
    using namespace std::literals;

    std::string_view config;
    bool bin_config = false;
    uint32_t deviceID = 0;
    if (argc >= 2) {
        config = argv[1];
        if (config.substr(config.size() - 4) == ".bin") {
            bin_config = true;
        }
        if (argc > 2) {
            deviceID = atoi(argv[2]);
        }
    } else {
        printf("Please specify the configuration file.\n");
        exit(-1);
    }

    Arducam::DeviceList device_list = Arducam::Camera::listDevices();
    const uint32_t device_list_size = device_list.size();
    printf("device list size: %d\n", device_list_size);
    if (deviceID >= device_list_size) {
        printf("Please select the correct device ID\n");
        return -1;
    }
    ArducamDeviceHandle device = device_list[deviceID];
    Arducam::Camera camera;
    Arducam::Param param;
    param.config_file_name = config.data();
    param.bin_config = bin_config;
    param.device = device;
    if (!camera.open(param)) {
        printf("Failed to open camera. ret = %x\n", camera.lastError());
        return -1;
    }
    if (!camera.init()) {
        printf("Failed to init camera. ret = %x\n", camera.lastError());
        return -1;
    }

    PrintDeviceInfo(camera, device);
    dumpDeviceInfo(camera);
    printf("width: %d, height: %d\n", camera.width(), camera.height());

    camera.start();

    while (!exit_flag) {
        ArducamImageFrame image;
        if (!camera.capture(image, 1000) || image.data == nullptr) {
            printf("Error reading frame.\n");
            continue;
        }
        show_image(camera, image);
        camera.freeImage(std::move(image));
    }

    camera.close();

    cv::destroyAllWindows();
    return 0;
}
