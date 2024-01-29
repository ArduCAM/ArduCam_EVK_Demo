import argparse

import cv2
from ArducamEvkSDK import Camera, Param, Frame

from utils import show_image, WaitGroup


def main(config):
    param = Param()
    param.config_file_name = config
    param.bin_config = config.endswith(".bin")
    camera = Camera()
    r = camera.open(param)
    if not r:
        raise Exception("open camera error! ret={}".format(camera.last_error))
    camera.init()
    group = WaitGroup(1)

    def process_image(image):
        show_image(image)
        key = cv2.waitKey(1)
        if key == ord("q"):
            group.done()

    camera.set_capture_callback(process_image)
    camera.start()
    print("camera.width={}, camera.height={}".format(camera.config.width, camera.config.height))
    group.wait()
    camera.stop()
    camera.close()


if __name__ == "__main__":
    parse = argparse.ArgumentParser(description="Arducam Evk SDK example.")
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
    )

    args = parse.parse_args()

    main(args.config)
