import argparse
# from typing import Generator, Tuple

from ArducamEvkSDK import Camera, Param, get_error_name


def to_info(sensor):
    """
    @rtype: Generator[Tuple[int, str], Any, None]
    """
    for pix in sensor['pix_formats']:
        for res in pix['resolutions']:
            yield res['id'], "{}x{}".format(res['width'], res['height'])


def main(config_path, mode_id, only_list):
    """
    @type config_path: str
    @type mode_id: int
    @type only_list: bool
    """
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = True  # list mode only support bin config
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception("open camera error! {}".format(get_error_name(camera.last_error)))  # get the last error message
    if only_list:
        # get the bin config
        config = camera.bin_config
        # print sensor info
        print("Sensor info:")
        for id_, info in to_info(config):
            print("{}: {}".format(id_, info))
    else:
        camera.init()  # init camera
        # print current resolution
        print("Current resolution: {}x{}".format(camera.config.width, camera.config.height))
        # switch to sensor mode
        if not camera.switch_mode(mode_id):
            # if failed, raise exception
            raise Exception("switch mode error! {}".format(get_error_name(camera.last_error)))
        print("Switch to mode_id={}".format(mode_id))
        # print current resolution
        print("Current resolution: {}x{}".format(camera.config.width, camera.config.height))

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
        "-i", "--id",
        help="Sensor Mode ID.",
        type=int,
        default=1,
    )
    parse.add_argument(
        "-l", "--list",
        help="List all sensor mode.",
        action="store_true",
    )

    args = parse.parse_args()

    main(args.config, args.id, args.list)
