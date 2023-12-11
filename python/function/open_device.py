import argparse

from ArducamEvkSDK import *


def main(config_path, device_index):
    """
    @type device_index: int
    @type config_path: str
    """
    camera = Camera()
    param = Param()
    devs = list_devices()  # get all devices
    device_index -= 1  # make the index start from 0
    if device_index < 0:
        raise Exception("device index must be greater than 1.")
    if device_index >= len(devs):
        raise Exception("device index out of range, total {} devices.".format(len(devs)))
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    param.device = devs[device_index]  # set the opened device
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception("open camera error! {}".format(get_error_name(camera.last_error)))  # get the last error message
    camera.init()  # init camera
    # ...
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
        "-d", "--device",
        help="Device index. Starts from 1. Default is 1.",
        type=int,
        default=1,
    )

    args = parse.parse_args()

    main(args.config, args.device)
