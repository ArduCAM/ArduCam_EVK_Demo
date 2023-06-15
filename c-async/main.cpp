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

    // display frame with opencv
    show_buffer(fb);
}

int main(int argc, char **argv) {
    using namespace std::literals;

    const char *config_file_name;
    bool bin_config = false;
    if (argc == 2) {
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

    ArducamRegisterReadCallback(handle, show_image, nullptr);
    ArducamStartCamera(handle);

    {
        std::unique_lock lk(mtx);
        while (!exit_flag) {
            if (key == 'q') {
                exit_flag = true;
                break;
            }
            key = 0;
            if (!event_cond.wait_for(lk, 500ms, [] { return key != 0; })) {
                key = -1;
                // key = cv::pollKey();
            }
        }
        // exit_cond.wait(lk, [] { return exit_flag; });
    }

    ArducamCloseCamera(handle);

    cv::destroyAllWindows();
    return 0;
}
