import argparse
from typing import Generator, Tuple, Any

from ArducamEvkSDK import Camera, Param, get_error_name

from sensor_mode import SensorInfo


def to_info(sensor: SensorInfo) -> Generator[Tuple[int, str], Any, None]:
    for pix in sensor.pix_formats:
        for res in pix.resolutions:
            yield res.id, f"{pix.type.name} {res.width}x{res.height}"


def main(config_path: str, mode_id: int, only_list: bool):
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = True  # list mode only support bin config
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception(f"open camera error! {get_error_name(camera.last_error)}")  # get the last error message
    if only_list:
        # get the bin config
        config = camera.bin_config
        # convert to SensorInfo
        sensor: SensorInfo = SensorInfo.parse_obj(config)
        # print sensor info
        print("Sensor info:")
        for id_, info in to_info(sensor):
            print(f"{id_}: {info}")
    else:
        camera.init()  # init camera
        # print current resolution
        print(f"Current resolution: {camera.config.width}x{camera.config.height}")
        # switch to sensor mode
        if not camera.switch_mode(mode_id):
            # if failed, raise exception
            raise Exception(f"switch mode error! {get_error_name(camera.last_error)}")
        print(f"Switch to mode_id={mode_id}")
        # print current resolution
        print(f"Current resolution: {camera.config.width}x{camera.config.height}")

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
