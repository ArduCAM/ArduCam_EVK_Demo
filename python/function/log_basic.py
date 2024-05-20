from ArducamEvkSDK import Camera, Param, get_error_name, LoggerLevel, CameraConfig


def main():
    camera = Camera()
    param = Param()
    if not camera.open(param):  # open camera, return True if success, otherwise return False
        raise Exception("open camera error! {}".format(get_error_name(camera.last_error)))  # get the last error message
    print("Current Log Level: {}".format(camera.log_level))  # get current log level
    print("Set the Log Level to Trace")
    camera.log_level = LoggerLevel.Trace  # set log level to info
    print("Current Log Level: {}".format(camera.log_level))  # get current log level

    # set camera config
    camera_config = CameraConfig()
    camera_config.height = 1080  # set height (required)
    camera_config.width = 1920  # set width (required)
    camera.config = camera_config  # set config to camera

    # init camera
    camera.init()

    print("Disable Console Log")
    camera.enable_console_log(False)  # disable console log
    camera.close()


if __name__ == '__main__':
    main()
