from ArducamEvkSDK import list_devices, Device


def to_name(dev: Device):
    res = [chr(i) for i in dev.serial_number[:12]]
    res.insert(8, "-")
    res.insert(4, "-")
    return "".join(res)


def main():
    devs = list_devices()
    for i in devs:
        print(f"{i.id_vendor}:{i.id_product} {to_name(i)}; usb type: {i.usb_type:04x}")


if __name__ == "__main__":
    main()
