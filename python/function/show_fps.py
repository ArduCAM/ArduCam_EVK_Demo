import argparse
import threading
import time

from ArducamEvkSDK import Camera, Param, get_error_name


def main(config_path):
    """
    @type config_path: str
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
    camera.start()

    # wait 2 seconds
    def wait():
        time.sleep(2)

        # show fps and bandwidth
        print(
            "fps: {}, bandwidth: {}B/s ({:.2f}MB/s)".format(
                camera.capture_fps, camera.bandwidth, camera.bandwidth / 1024 / 1024
            )
        )

        camera.stop()
        camera.close()

    thread = threading.Thread(target=wait)
    thread.start()
    thread.join()


if __name__ == "__main__":
    parse = argparse.ArgumentParser()
    parse.add_argument(
        "-c",
        "--config",
        help="Path to config file.",
        type=str,
        required=True,
    )

    args = parse.parse_args()

    main(args.config)
