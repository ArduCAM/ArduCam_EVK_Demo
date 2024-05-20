from ArducamEvkSDK import DeviceList


def main():
    dev_lst = DeviceList()
    devs = dev_lst.devices()
    print("Total {} devices.".format(len(devs)))
    for index, dev in enumerate(devs, 1):
        print("{}: {}".format(index, dev))


if __name__ == "__main__":
    main()
