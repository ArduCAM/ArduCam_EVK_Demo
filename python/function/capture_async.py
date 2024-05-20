import argparse
import threading
import time

from ArducamEvkSDK import Camera, Param, get_error_name


def callback(image):
    """
    @type image: Frame
    """
    print("get frame({}x{}) from camera.".format(image.format.width, image.format.height))


def main(config_path, delay):
    """
    @type config_path: str
    @type delay: float
    """
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception("open camera error! {}".format(get_error_name(camera.last_error)))  # get the last error message
    camera.init()  # init camera

    # set read callback, this should before camera.start()
    camera.set_capture_callback(callback)

    camera.start()

    def wait():
        time.sleep(delay)

        camera.stop()
        camera.close()

    thread = threading.Thread(target=wait)
    thread.start()
    thread.join()


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

    main(args.config, args.delay)
