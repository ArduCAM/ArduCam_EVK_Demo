# C Functions

## Build

### Linux

```
mkdir -p build && cd build && cmake ..
make -j
```

### Windows

```
cmake -Bbuild .
cd build
cmake --build . --config Release
```

## Run

### Capture

> Capture images synchronously.

Usage: `./capture [-c <file>] [-n <int>] [--help]`

-c, --config=\<file>      Path to config file.
-n, --take=\<int>         Number of frames to take.
--help                    display this help and exit

### Capture async

> Capture images asynchronously.

Usage: `./capture_async [-c <file>] [-d <double>] [--help]`

-c, --config=\<file>      Path to config file.
-d, --delay=\<double>     Delay time in seconds.
--help                    display this help and exit

### Controls

> Show all controls.

Usage: `./controls [-c <file>] [--help]`

-c, --config=\<file>      Path to config file.
--help                    display this help and exit

### Devices hotplug

> Monitor device hotplug events.

Usage: `./devices_hotplug [-d <double>] [--help]`

-d, --delay=\<double>     Delay time in seconds.
--help                    display this help and exit

### Dump info

> Dump camera information.

Usage: `./dump_info [-c <file>] [--help]`

-c, --config=\<file>      Path to config file.
--help                    display this help and exit

### List devices

> List all supported devices.

Usage: `./list_devices`

### List mode

> List sensor mode or switch to sensor mode.

Usage: `./list_mode [-l] [-c <file>] [-i <int>] [--help]`

-c, --config=\<file>      Path to config file.
-i, --id=\<int>           Sensor Mode ID.
-l, --list                List all sensor mode.
--help                    display this help and exit

### Log basic

> Log basic usage.

Usage: `./log_basic`

### Log callback

> Log with callback.

Usage: `./log_callback`

### Log file

> Log to file.

Usage: `./log_file`

### Open advanced

> Open camera with advanced options.

Usage: `./open_advanced [-d] [-c <file>] [--help]`

-c, --config=\<file>      Path to config file.
-d, --dma                 Enable DMA.
--help                    display this help and exit

### Open basic

> Open camera with basic options.

Usage: `./open_basic [-c <file>] [--help]`

-c, --config=\<file>      Path to config file.
--help                    display this help and exit

### Open device

> Open camera with device index.

Usage: `./open_device [-c <file>] [-d <int>] [--help]`

-c, --config=\<file>      Path to config file.
-d, --device=\<int>       Device index. Starts from 1. Default is 1.
--help                    display this help and exit

### Set transfer option

> Set transfer option.

Usage: `./set_transfer_option [-c <file>] [-t <int>] [-s <int>] [-n <int>] [--help]`

-c, --config=\<file>      Path to config file.
-t, --count=\<int>        Transfer count.
-s, --size=\<int>         Transfer size.
-n, --take=\<int>         Number of frames to take.
--help                    display this help and exit

### Show fps

> Show FPS and bandwidth.

Usage: `./show_fps [-c <file>] [--help]`

-c, --config=\<file>      Path to config file.
--help                    display this help and exit
