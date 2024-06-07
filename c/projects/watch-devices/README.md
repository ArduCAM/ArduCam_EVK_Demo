# Introduction

This is a demo for watching devices.

## Build

### Windows

Enter the evk repository `ArduCam_EVK_Demo\c\projects\watch-devices` directory in the command line

```
cmake -Bbuild .
cd build
cmake --build . --config Release
```

After compilation, you can find the demo_c_watch-devices.exe file in the `ArduCam_EVK_Demo\c\projects\watch-devices\build\Release` directory and open it in the current directory cmd, and execute
```
.\demo_c_watch_devices.exe
```

### Linux

```
cd c/projects/watch-devices
mkdir -p build && cd build && cmake ..
make -j

./demo_c_watch_devices
```
