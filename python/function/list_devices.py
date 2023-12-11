from ArducamEvkSDK import list_devices


def main():
    devs = list_devices()
    print("Total {} devices.".format(len(devs)))
    for index, dev in enumerate(devs, 1):
        print("{}: {}".format(index, dev))


if __name__ == "__main__":
    main()
