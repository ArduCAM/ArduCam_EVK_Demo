from ArducamEvkSDK import Camera, Param, get_error_name, LoggerLevel, CameraConfig


def log_callback(level, msg):
    """
    @type level: LoggerLevel
    @type msg: str
    """
    print("[{}] {}".format(level.name, msg))


def main():
    camera = Camera()
    param = Param()
    # open camera, return True if success, otherwise return False
    if not camera.open(param):
        # get the last error message
        raise Exception(
            "open camera error! {}".format(get_error_name(camera.last_error))
        )
    print("Disable Console Log")
    camera.set_message_callback(log_callback)  # set log callback
    camera.enable_console_log(False)  # disable console log
    camera.log_level = LoggerLevel.Trace  # set log level to info

    # set camera config
    camera_config = CameraConfig()
    camera_config.height = 1080  # set height (required)
    camera_config.width = 1920  # set width (required)
    camera.config = camera_config  # set config to camera

    # init camera
    camera.init()
    camera.close()


if __name__ == "__main__":
    main()
