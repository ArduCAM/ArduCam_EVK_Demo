import argparse

from ArducamEvkSDK import Camera, Param, get_error_name


def dumpDeviceInfo(camera):
    USB_CPLD_I2C_ADDRESS = 0x46
    cpld_info = {}
    version = camera.read_reg_8_8(USB_CPLD_I2C_ADDRESS, 0x00)
    year = camera.read_reg_8_8(USB_CPLD_I2C_ADDRESS, 0x05)
    mouth = camera.read_reg_8_8(USB_CPLD_I2C_ADDRESS, 0x06)
    day = camera.read_reg_8_8(USB_CPLD_I2C_ADDRESS, 0x07)

    cpld_info["version"] = "v{}.{}".format(version >> 4, version & 0x0F)
    cpld_info["year"] = year
    cpld_info["mouth"] = mouth
    cpld_info["day"] = day

    print(cpld_info)

    data = camera.read_board_config(0x80, 0x00, 0x00, 2)
    if not data:
        return

    usb_info = {}
    usb_info["fw_version"] = "v{}.{}".format((data[0] & 0xFF), (data[1] & 0xFF))
    usb_info["usb_type"] = camera.usb_type

    print(usb_info)


def main(config_path):
    """
    @type config_path: str
    """
    camera = Camera()
    param = Param()
    param.config_file_name = config_path  # a path of config file
    param.bin_config = config_path.endswith(".bin")  # if the config file is a bin file
    if not camera.open(
        param
    ):  # open camera, return True if success, otherwise return False
        raise Exception(
            "open camera error! {}".format(get_error_name(camera.last_error))
        )  # get the last error message
    camera.init()  # init camera
    dumpDeviceInfo(camera)
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

    args = parse.parse_args()

    main(args.config)
