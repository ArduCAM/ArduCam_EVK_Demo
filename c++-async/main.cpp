#include <stdio.h>

#include <condition_variable>
#include <iostream>
#include <string>
#include <string_view>

#include "utils.h"

bool exit_flag = false;
uint8_t key = 0;
std::mutex mtx;
std::condition_variable event_cond;

void show_image(Arducam::Camera &camera, ArducamFrameBuffer fb) {
    int key = cv::waitKey(1);
    if (key != -1) {
        ::key = key;
        event_cond.notify_one();
    }

    show_buffer(fb);
}

int main(int argc, char **argv) {
    using namespace std::literals;

    std::string_view config;
    bool bin_config = false;
    if (argc > 1) {
        config = argv[1];
        if (config.substr(config.size() - 4) == ".bin") {
            bin_config = true;
        }
    } else {
        printf("Please specify the configuration file.\n");
        exit(-1);
    }

    Arducam::Camera camera;
    Arducam::OpenParam param;
    param.config_file_name = config.data();
    param.bin_config = bin_config;
    if (!camera.open(param)) {
        printf("Failed to open camera. ret = %x\n", camera.lastError());
        return -1;
    }
    if (!camera.init()) {
        printf("Failed to init camera. ret = %x\n", camera.lastError());
        return -1;
    }

    printf("width: %d, height: %d\n", camera.width(), camera.height());

    camera.setReadCallback(std::bind(show_image, std::ref(camera), std::placeholders::_1));
    camera.start();

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

    camera.close();

    cv::destroyAllWindows();
    return 0;
}
