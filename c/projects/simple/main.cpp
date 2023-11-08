#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

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
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);

    param.config_file_name = config_file_name;
    param.bin_config = bin_config;
    param.mem_type = DMA;
    param.device = device;

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

    ArducamCameraConfig config;
    ArducamGetCameraConfig(handle, &config);
    printf("width: %d, height: %d\n", config.width, config.height);

    ArducamStartCamera(handle);

    while (true) {
        ArducamImageFrame image;
        ret = ArducamCaptureImage(handle, &image, 1000);
        if (ret) {
            printf("Error reading frame.\n");
            continue;
        }
        show_image(image, "Test");

        int key = cv::waitKey(1);
        if (key == 'q') {
            break;
        }
        ArducamFreeImage(handle, image);
    }

    ArducamCloseCamera(handle);
    ArducamFreeDeviceList();

    cv::destroyAllWindows();
    return 0;
}
