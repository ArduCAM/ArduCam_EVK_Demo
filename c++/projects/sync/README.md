# Introduction

This is a demo for reading and displaying images with sync mode.

## Build

### Windows

Enter the evk repository `ArduCam_EVK_Demo\c++\projects\sync` directory in the command line

```
cmake -Bbuild .
cd build
cmake --build . --config Release
```

After compilation, you can find the demo_cpp_sync.exe file in the `ArduCam_EVK_Demo\c++\projects\sync\build\Release` directory and open it in the current directory cmd, and execute
```
.\demo_cpp_sync.exe <configuration file>
```

### Linux
<!-- git clone https://github.com/ArduCAM/ArduCAM_USB_Camera_Shield_Cpp_Demo.git -->
<!-- cd ArduCAM_USB_Camera_Shield_Cpp_Demo -->
```
cd c++/projects/sync
mkdir -p build && cd build && cmake ..
make -j

./demo_cpp_sync <path/config-file-name>
```
