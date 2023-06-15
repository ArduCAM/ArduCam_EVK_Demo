#include <stdio.h>

#include "utils.h"

bool exit_flag = false;

void show_image(ArducamCameraHandle handle, ArducamFrameBuffer fb) {
    int key = cv::waitKey(1);
    if (key == 'q') {
        exit_flag = true;
    }

    show_buffer(fb);
}

int main(int argc, char **argv) {
    using namespace std::literals;

    const char *config_file_name;
    bool bin_config = false;
    if (argc > 1) {
        config_file_name = argv[1];
        int len = strlen(config_file_name);
        if (len > 4) {
            const char *postfix = config_file_name + len - 4;
            if (strcmp(postfix, ".bin") == 0) {
                bin_config = true;
            }
        }
    } else {
        printf("Please specify the configuration file.\n");
        exit(-1);
    }

    int ret = 0;
    ArducamCameraHandle handle;
    ArducamCameraOpenParam param;
    ArducamDefaultParam(&param);
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

    ArducamCameraConfig config;
    ArducamGetCameraConfig(handle, &config);
    printf("width: %d, height: %d\n", config.width, config.height);

    ArducamStartCamera(handle);

    while (!exit_flag) {
        ArducamFrameBuffer fb;
        ArducamReadFrame(handle, &fb, 1000);
        if (fb.data == nullptr) {
            printf("Error reading frame.\n");
            continue;
        }
        show_image(handle, fb);
        ArducamReturnBuffer(handle, fb);
    }

    ArducamCloseCamera(handle);

    cv::destroyAllWindows();
    return 0;
}
