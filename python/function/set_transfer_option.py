import argparse
from typing import Optional, cast

from ArducamEvkSDK import Camera, Param, get_error_name, Frame


def main(config_path, count, size, num):
    """
    @type config_path: str
    @type count: int
    @type size: int
    @type num: int
    """
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    # open camera, return True if success, otherwise return False
    if not camera.open(param):
        # get the last error message
        raise Exception(
            "open camera error! {}".format(get_error_name(camera.last_error))
        )
    camera.init()  # init camera

    # set transfer option, this should before camera.start()
    camera.set_transfer(count, size)

    camera.start()
    for i in range(num):
        # the capture should return a Frame object or None
        image = cast(Optional[Frame], camera.capture(1000))
        if image is not None:
            print(
                "get frame({}x{}) from camera.".format(
                    image.format.width, image.format.height
                )
            )
    camera.stop()
    camera.close()


if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument(
        "-c",
        "--config",
        help="Path to config file.",
        type=str,
        required=True,
    )
    parse.add_argument(
        "-t",
        "--count",
        help="Transfer count.",
        type=int,
        default=30,
    )
    parse.add_argument(
        "-s",
        "--size",
        help="Transfer size.",
        type=int,
        default=512 * 1024,
    )
    parse.add_argument(
        "-n",
        "--take",
        help="Number of frames to take.",
        type=int,
        default=1,
    )

    args = parse.parse_args()

    main(args.config, args.count, args.size, args.take)
