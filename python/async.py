import argparse
import threading
import time

import cv2
from ArducamEvkSDK import Camera, Param

from utils import show_image, WaitGroup


def main(config):
    param = Param()
    param.config_file_name = config
    param.bin_config = config.endswith(".bin")
    camera = Camera()
    r = camera.open(param)
    if r != 0:
        raise Exception(f"open camera error! ret={hex(r)}")
    camera.init()
    group = WaitGroup(1)

    def show_image(image):
        show_image(image)
        key = cv2.waitKey(1)
        if key == ord("q"):
            group.done()

    camera.set_read_callback(show_image)
    camera.start()
    print(f"camera.width={camera.config.width}, camera.height={camera.config.height}")
    group.wait()
    camera.stop()
    camera.close()


if __name__ == "__main__":
    parse = argparse.ArgumentParser(description="Arducam SDK example.")
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
    )

    args = parse.parse_args()

    main(args.config)
