# Directory Structure
```
.
├── function # python demo source code (independent use cases for each function)
└── project # python demo project (integrated use cases for common functions)
```

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
python function/list_devices.py
```

### Open

Open Basic

```shell
# show help
python function/open_basic.py --help
# run
python function/open_basic.py -c <config>
```

Open Specific Device

```shell
# show help
python function/open_device.py --help
# run
python function/open_device.py -c <config> -d <device_index>
```

Open Advanced

```shell
# show help
python function/open_advanced.py --help
# run
python function/open_advanced.py -c <config> --dma
```

Set Transfer Option

```shell
# show help
python function/set_transfer_option.py --help
# run
python function/set_transfer_option.py -c <config> -t <transfer_count> -s <transfer_size> -n <number_of_frames>
```

### Log

Log Basic

```shell
# run
python function/log_basic.py
```

Set the environment variable `ARDUCAM_LOG_LEVEL` to change the log level before running.
The `ARDUCAM_LOG_LEVEL` value can be `off`, `error`, `warn`, `info`, `debug`, `trace`.

```shell
# run with environment variable
ARDUCAM_LOG_LEVEL=off python function/log_basic.py
```

```shell
# run with environment variable
ARDUCAM_LOG_LEVEL=trace python function/log_basic.py
```

Log Callback

```shell
# run
python function/log_callback.py
```

Log File

```shell
# run
python function/log_file.py
```

### Capture

Capture Sync

```shell
# show help
python function/capture.py --help
# run
python function/capture.py -c <config> -n <number_of_frames>
```

Capture Async

```shell
# show help
python function/capture_async.py --help
# run
python function/capture_async.py -c <config> -d <delay_in_seconds>
```

### Utils

Show fps

```shell
# show help
python function/show_fps.py --help
# run
python function/show_fps.py -c <config>
```

List All Modes in Binary Config

> Make sure you have a camera connected

```shell
# show help
python function/list_mode.py --help
# run, and list all modes in binary config
python function/list_mode.py -c <config> -l
# run, and switch to a specific mode
python function/list_mode.py -c <config> -i <mode_id>
```

Controls

> Liat all controls

```shell
# show help
python function/controls.py --help
# run
python function/controls.py -c <config>
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
