from ArducamEvkSDK import list_devices


def main():
    devs = list_devices()
    print(f"Total {len(devs)} devices.")
    for index, dev in enumerate(devs, 1):
        print(f"{index}: {dev}")


if __name__ == "__main__":
    main()
