#include <stdio.h>

#include <condition_variable>

#include "utils.h"

bool exit_flag = false;
int key = 0;
std::mutex mtx;
std::condition_variable event_cond;

void show_image(ArducamFrameBuffer fb, void *user_data) {
    int key = cv::waitKey(1);
    if (key != -1) {
        ::key = key;
        event_cond.notify_one();
    }

    printf("image[%d] with resolution: %d x %d\n", fb.seq, fb.format.width, fb.format.height);
    // display frame with opencv
    show_buffer(fb);
}

void event_process(ArducamEventCode event, void *user_data) {
    switch (event)
    {
    case FrameStart:
        printf("Frame start\n");
        break;
    case FrameEnd:
        printf("Frame end\n");
        break;
    default:
        break;
    }
}

void error_process(ArducamLoggerLevel type, const char *error, void *user_data) {
    printf("[Error] %s\n", error);
}

void PrintDeviceInfo(ArducamCameraHandle handle, ArducamDeviceHandle device) {
    printf("device id vendor: 0x%04X\n", device->id_vendor);
    printf("device id product: 0x%04X\n", device->id_product);
    printf("device in used: %d\n", device->in_used);

    const uint8_t *serial_number = device->serial_number;
    printf("device serial number: %c%c%c%c-%c%c%c%c-%c%c%c%c\n",
			serial_number[0], serial_number[1], serial_number[2], serial_number[3],
			serial_number[4], serial_number[5], serial_number[6], serial_number[7],
			serial_number[8], serial_number[9], serial_number[10], serial_number[11]);

    printf("device usb type: %s\n", ArducamGetUSBType(handle));
    printf("device speed: %d\n", device->speed);
}

#define USB_CPLD_I2C_ADDRESS 0x46
void dumpDeviceInfo(ArducamCameraHandle handle) {
	uint32_t ret = 0, version = 0, year = 0, mouth = 0, day = 0;
	ret = ArducamReadReg_8_8(handle, USB_CPLD_I2C_ADDRESS, 0x00, &version);
	ret = ArducamReadReg_8_8(handle, USB_CPLD_I2C_ADDRESS, 0x05, &year);
	ret = ArducamReadReg_8_8(handle, USB_CPLD_I2C_ADDRESS, 0x06, &mouth);
	ret = ArducamReadReg_8_8(handle, USB_CPLD_I2C_ADDRESS, 0x07, &day);

	printf("CPLD version: v%d.%d date: 20%d-%02d-%02d\n", version >> 4, version & 0x0F, year, mouth, day);
	uint8_t data[16];
	ret = ArducamReadBoardConfig(handle, 0x80, 0x00, 0x00, 2, data);
	printf("fw_version: v%d.%d \n", data[0] & 0xFF, data[1] & 0xFF);
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
        .config_file_name = config_file_name,
        .bin_config = bin_config,
        .mem_type = DMA,
        .device = device
    };
    
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
    dumpDeviceInfo(handle);

    ArducamEnableConsoleLog(handle);
    ArducamSetDebugLevel(handle, info);

    ArducamCameraConfig config;
    ArducamGetCameraConfig(handle, &config);
    printf("width: %d, height: %d\n", config.width, config.height);

    // const Control* control_list;
    // uint32_t controls_length;
    // ret = ArducamListCtrls(handle, &control_list, &controls_length);
    // if (!ret) {
    //     for (uint32_t i = 0; i < controls_length; i++) {
    //         printf("Control Name: %s\n", control_list[i].func);
    //     }
    // }

    // ArducamSetCtrl(handle, "setFramerate", 30);
    // ArducamSetCtrl(handle, "setExposureTime", 10000);

    ArducamRegisterReadCallback(handle, show_image, nullptr);
    // ArducamRegisterEventCallback(handle, event_process, nullptr);
    ArducamRegisterMessageCallback(handle, error_process, nullptr);
    ArducamStartCamera(handle);
    uint32_t mode_id = 1;
    {
        std::unique_lock lk(mtx);
        while (!exit_flag) {
            if (key == 'q') {
                exit_flag = true;
                break;
            } else if (key == 's' && !ArducamBinConfigLoaded(handle)) {
                mode_id++;
                if (mode_id > 3) {
                    mode_id = 1;
                }
                ArducamSwitchMode(handle, mode_id);
            }


            key = 0;
            if (!event_cond.wait_for(lk, 500ms, [] { return key != 0; })) {
                key = -1;
                // key = cv::pollKey();
            }

            // printf("fps: %d bandwidth: %d\n", ArducamGetCaptureFps(handle), ArducamGetBandwidth(handle));
        }
        // exit_cond.wait(lk, [] { return exit_flag; });
    }

    ArducamCloseCamera(handle);
    ArducamFreeDeviceList();

    cv::destroyAllWindows();
    return 0;
}
