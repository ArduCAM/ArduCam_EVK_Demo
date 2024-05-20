import argparse
import asyncio

from ArducamEvkSDK import Camera, Param, get_error_name, Frame


def callback(image: Frame):
    print(f"get frame({image.format.width}x{image.format.height}) from camera.")


async def main(config_path: str, delay: float):
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception(f"open camera error! {get_error_name(camera.last_error)}")  # get the last error message
    camera.init()  # init camera

    # set read callback, this should before camera.start()
    camera.set_capture_callback(callback)

    camera.start()
    # use asyncio.sleep to wait for `delay` seconds
    await asyncio.sleep(delay)
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
        "-d", "--delay",
        help="Delay time in seconds.",
        type=float,
        default=1.0,
    )

    args = parse.parse_args()

    asyncio.run(main(args.config, args.delay))
