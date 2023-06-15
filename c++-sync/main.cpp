#include <stdio.h>

#include <iostream>
#include <string>
#include <string_view>

#include "utils.h"

bool exit_flag = false;

void show_image(Arducam::Camera &camera, ArducamFrameBuffer fb) {
    int key = cv::waitKey(1);
    if (key == 'q') {
        exit_flag = true;
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

    camera.start();

    while (!exit_flag) {
        ArducamFrameBuffer fb;
        camera.read(fb, 1000);
        if (fb.data == nullptr) {
            printf("Error reading frame.\n");
            continue;
        }
        show_image(camera, fb);
        camera.returnBuffer(std::move(fb));
    }

    camera.close();

    cv::destroyAllWindows();
    return 0;
}
