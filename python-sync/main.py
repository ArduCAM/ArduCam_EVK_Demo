import argparse

import cv2
from ArducamEvkSDK import Camera, Param, LOG_OFF, LOG_INFO

from utils import show_buffer


def main(config):
    param = Param()
    param.config_file_name = config
    param.bin_config = config.endswith(".bin")
    camera = Camera()
    r = camera.open(param)
    camera.log_level = LOG_INFO
    print(camera.usb_type)
    if r != 0:
        raise Exception(f"open camera error! ret={r}")
    camera.init()
    camera.start()
    print(f"{camera.width=}, {camera.height=}")
    while True:
        fb = camera.read(1000)
        if fb is None:
            continue
        show_buffer(fb)
        # print(f"get frame({fb.format.width}x{fb.format.height}) from camera.")
        key = cv2.waitKey(1)
        if key == ord('q'):
            break
    camera.stop()
    camera.close()


if __name__ == "__main__":
    parse = argparse.ArgumentParser(description="Arducam SDK example.")
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
        required=True,
    )

    args = parse.parse_args()

    main(args.config)
