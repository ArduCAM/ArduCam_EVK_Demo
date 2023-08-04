import argparse
import asyncio

from ArducamEvkSDK import *


async def main(config_path: str):
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception(f"open camera error! {get_error_name(camera.last_error)}")  # get the last error message
    camera.init()  # init camera
    camera.start()
    # wait 2 seconds
    await asyncio.sleep(2)

    # show fps and bandwidth
    print(f"fps: {camera.capture_fps}, bandwidth: {camera.bandwidth}B/s ({camera.bandwidth / 1024 / 1024:.2f}MB/s)")

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

    args = parse.parse_args()

    asyncio.run(main(args.config))
