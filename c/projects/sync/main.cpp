#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

bool exit_flag = false;
uint32_t mode_id = 1;
uint32_t mode_size = 0;

void show_image(ArducamCameraHandle handle, ArducamImageFrame image) {
    int key = cv::waitKey(1);
    if (key == 'q') {
        exit_flag = true;
    } else if (key == 's' && !ArducamBinConfigLoaded(handle)) {
        mode_id++;
        if (mode_id > mode_size) {
            mode_id = 1;
        }
        ArducamSwitchMode(handle, mode_id);
    }

    printf("image[%d] with resolution: %d x %d\n", image.seq, image.format.width, image.format.height);

    show_image(image);
}

void PrintDeviceInfo(ArducamCameraHandle handle, ArducamDeviceHandle device) {
    printf("device id vendor: 0x%04X\n", device->id_vendor);
    printf("device id product: 0x%04X\n", device->id_product);
    printf("device in used: %d\n", device->in_used);

    const uint8_t *serial_number = device->serial_number;
    printf("device serial number: %c%c%c%c-%c%c%c%c-%c%c%c%c\n", serial_number[0], serial_number[1], serial_number[2],
           serial_number[3], serial_number[4], serial_number[5], serial_number[6], serial_number[7], serial_number[8],
           serial_number[9], serial_number[10], serial_number[11]);

    printf("device usb type: %s\n", ArducamGetUSBType(handle));
    printf("device speed: %d\n", device->speed);
}

int main(int argc, char **argv) {
    using namespace std::literals;

    const char *config_file_name;
    bool bin_config = false;
    uint32_t deviceID = 0;
    if (argc >= 2) {
        config_file_name = argv[1];
        int len = strlen(config_file_name);
        if (len > 4) {
            const char *postfix = config_file_name + len - 4;
            if (strcmp(postfix, ".bin") == 0) {
                bin_config = true;
            }
        }
        if (argc > 2) {
            deviceID = atoi(argv[2]);
        }
    } else {
        printf("Please specify the configuration file.\n");
        exit(-1);
    }

    int ret = 0;

    ArducamDeviceListHandle device_list;
    ret = ArducamListDevice(&device_list);
    if (ret) {
        printf("Failed to get device list\n");
        return -1;
    }

    const uint32_t device_list_size = device_list->size;
    printf("device list size: %d\n", device_list_size);
    if (deviceID >= device_list_size) {
        printf("Please select the correct device ID\n");
        return -1;
    }

    ArducamDeviceHandle device = device_list->devices[deviceID];
    ArducamCameraHandle handle;
    ArducamCameraOpenParam param = {
        .config_file_name = config_file_name, .bin_config = bin_config, .mem_type = DMA, .device = device};

    // ArducamDefaultParam(&param);

    param.config_file_name = config_file_name;
    param.bin_config = bin_config;
    ret = ArducamOpenCamera(&handle, &param);
    if (ret) {
        printf("Failed to open camera. ret = %x\n", ret);
        return -1;
    }
    ret = ArducamInitCamera(handle);
    if (ret) {
        printf("Failed to init camera. ret = %x\n", ret);
        return -1;
    }

    PrintDeviceInfo(handle, device);

    if (ArducamBinConfigLoaded(handle)) {
        ArducamCameraConfig config;
        ArducamGetCameraConfig(handle, &config);
        printf("width: %d, height: %d\n", config.width, config.height);
    } else {
        ArducamCameraConfig *configs;
        uint32_t *ids;
        ret = ArducamListMode(handle, &configs, &ids, &mode_size);
        if (ret) {
            printf("Failed to get schema information\n");
            return -1;
        }
        printf("Mode size: %d\n", mode_size);
        ArducamFreeModeList(handle, configs, ids);
    }

    ArducamStartCamera(handle);

    while (!exit_flag) {
        ArducamImageFrame image;
        ArducamCaptureImage(handle, &image, 1000);
        if (image.data == nullptr) {
            printf("Error reading frame.\n");
            continue;
        }
        show_image(handle, image);
        ArducamFreeImage(handle, image);
    }

    ArducamCloseCamera(handle);

    cv::destroyAllWindows();
    return 0;
}
