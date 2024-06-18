import argparse

import cv2
import ArducamEvkSDK
from ArducamEvkSDK import Camera, Param

from img_cvt_utils import show_image, WaitGroup


def main(config):
    print("ArducamEvkSDK Version: {}".format(ArducamEvkSDK.__version__))

    param = Param()
    param.config_file_name = config
    param.bin_config = config.endswith(".bin")
    camera = Camera()
    r = camera.open(param)
    if not r:
        raise Exception("open camera error! ret={}".format(camera.last_error))
    
    print("SerialNumber: " + ''.join(chr(i) for i in camera.device.serial_number))
    print("DeviceType: {}".format(camera.usb_type))
    print("DeviceSpeed: {}".format(camera.device.speed))
    print("VID:PID: {:04x}:{:04x}".format(camera.device.id_vendor, camera.device.id_product))

    camera.init()
    group = WaitGroup(1)

    def process_image(image):
        show_image(image)
        key = cv2.waitKey(1)
        if key == ord("q"):
            group.done()

    camera.set_capture_callback(process_image)
    camera.start()
    print("camera.width={}, camera.height={}".format(camera.config.width, camera.config.height))
    group.wait()
    camera.stop()
    camera.close()


if __name__ == "__main__":
    parse = argparse.ArgumentParser(description="Arducam Evk SDK example.")
    parse.add_argument(
        "-c", "--config",
        help="Path to config file.",
        type=str,
    )

    args = parse.parse_args()

    main(args.config)
