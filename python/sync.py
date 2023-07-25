import argparse

import cv2
from ArducamEvkSDK import Camera, Param, LOG_OFF, LOG_INFO

from utils import show_image


def main(config):
    camera = Camera()
    param = Param()
    param.config_file_name = config
    param.bin_config = config.endswith(".bin")
    camera.open(param)
    camera.set_message_callback(lambda l, msg: print(msg))
    camera.log_level = LOG_INFO
    print(camera.usb_type)
    # if r != 0:
    #     raise Exception(f"open camera error! ret={r}")
    camera.init()
    camera.start()
    config = camera.config
    print(f"{config.width=}, {config.height=}")
    while True:
        image = camera.read(1000)
        if image is None:
            continue
        show_image(image)
        # print(f"get frame({image.format.width}x{image.format.height}) from camera.")
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
