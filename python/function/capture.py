import argparse
from typing import Optional, cast

from ArducamEvkSDK import *


def main(config_path: str, num: int):
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception(f"open camera error! {get_error_name(camera.last_error)}")  # get the last error message
    camera.init()  # init camera
    camera.start()
    for i in range(num):
        # the capture should return a Frame object or None
        image = cast(Optional[Frame], camera.capture(1000))
        if image is not None:
            print(f"get frame({image.format.width}x{image.format.height}) from camera.")
    camera.stop()
    camera.close()


if __name__ == '__main__':
    parse = argparse.ArgumentParser()
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
        required=True,
    )
    parse.add_argument(
        "-n", "--take",
        help="Number of frames to take.",
        type=int,
        default=1,
    )

    args = parse.parse_args()

    main(args.config, args.take)
