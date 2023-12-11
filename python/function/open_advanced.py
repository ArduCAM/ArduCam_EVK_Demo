import argparse
from typing import Optional

from ArducamEvkSDK import *


def main(config_path, dma_enable=True):
    """
    @type config_path: Optional[str]
    @type dma_enable: bool
    """
    camera = Camera()
    param = Param()
    # the config file is optional.
    # if not set, you must set the register value by yourself, after open camera and before init camera
    if config_path is not None:
        param.config_file_name = config_path  # a path of config file
        param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    param.mem_type = MemType.DMA if dma_enable else MemType.RAM  # set memory type
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception("open camera error! {}".format(get_error_name(camera.last_error)))  # get the last error message
    if config_path is None:
        camera_config = CameraConfig()
        camera_config.height = 1080  # set height (required)
        camera_config.width = 1920  # set width (required)
        camera.config = camera_config  # set config to camera
        # ...  # set register value by yourself
    camera.init()  # init camera
    # ...
    camera.close()


if __name__ == '__main__':
    parse = argparse.ArgumentParser()
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
        required=False,
    )
    parse.add_argument(
        "-d", "--dma",
        help="Enable DMA.",
        action="store_true",
    )

    args = parse.parse_args()

    main(args.config, args.dma)
