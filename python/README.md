# Introduction

This is a demo for reading and displaying images with sync/async mode.

## Installation dependence

<!-- git clone https://github.com/ArduCAM/ArduCAM_USB_Camera_Shield_Cpp_Demo.git -->
<!-- cd ArduCAM_USB_Camera_Shield_Cpp_Demo -->

```shell
cd python
pip install -r requirements.txt
```

## Functions

### List Devices

```shell
# run
python -m function.list_devices
```

### Open

Open Basic

```shell
# show help
python -m function.open_basic --help
# run
python -m function.open_basic -c <config>
```

Open Specific Device

```shell
# show help
python -m function.open_device --help
# run
python -m function.open_device -c <config> -d <device_index>
```

Open Advanced

```shell
# show help
python -m function.open_advanced --help
# run
python -m function.open_advanced -c <config> --dma
```

Set Transfer Option

```shell
# show help
python -m function.set_transfer_option --help
# run
python -m function.set_transfer_option -c <config> -t <transfer_count> -s <transfer_size> -n <number_of_frames>
```

### Log

Log Basic

```shell
# run
python -m function.log_basic
```

Set the environment variable `ARDUCAM_LOG_LEVEL` to change the log level before running.
The `ARDUCAM_LOG_LEVEL` value can be `off`, `error`, `warn`, `info`, `debug`, `trace`.

```shell
# run with environment variable
ARDUCAM_LOG_LEVEL=off python -m function.log_basic
```

```shell
# run with environment variable
ARDUCAM_LOG_LEVEL=trace python -m function.log_basic
```

Log Callback

```shell
# run
python -m function.log_callback
```

Log File

```shell
# run
python -m function.log_file
```

### Capture

Capture Sync

```shell
# show help
python -m function.capture --help
# run
python -m function.capture -c <config> -n <number_of_frames>
```

Capture Async

```shell
# show help
python -m function.capture_async --help
# run
python -m function.capture_async -c <config> -d <delay_in_seconds>
```

### Utils

Show fps

```shell
# show help
python -m function.show_fps --help
# run
python -m function.show_fps -c <config>
```

List All Modes in Binary Config

> Make sure you have a camera connected

```shell
# show help
python -m function.list_mode --help
# run, and list all modes in binary config
python -m function.list_mode -c <config> -l
# run, and switch to a specific mode
python -m function.list_mode -c <config> -i <mode_id>
```

## Project

### Run sync mode

Show help

```shell
python -m project.sync --help
```

Run

```shell
python -m project.sync -c <path/config-file-name>
```

### Run async mode

Show help

```shell
python -m project.async --help
```

Run

```shell
python -m project.async -c <path/config-file-name>
```
