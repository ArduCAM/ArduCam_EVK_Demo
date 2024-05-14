from ArducamEvkSDK import list_devices


def main():
    devs = list_devices()
    print("Total {} devices.".format(len(devs)))
    for index, dev in enumerate(devs, 1):
        print("{}: [{:04x}:{:04x}]".format(index, dev.id_vendor, dev.id_product))


if __name__ == "__main__":
    main()
