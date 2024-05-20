import argparse

from ArducamEvkSDK import Camera, Param, get_error_name



def main(config_path):
    """
    @type config_path: str
    """
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception("open camera error! {}".format(get_error_name(camera.last_error)))  # get the last error message
    camera.init()  # init camera
    ...
    camera.close()


if __name__ == '__main__':
    parse = argparse.ArgumentParser()
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
        required=True,
    )

    args = parse.parse_args()

    main(args.config)
