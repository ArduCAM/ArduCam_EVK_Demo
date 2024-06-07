# Introduction

This is a demo for reading and displaying images with async mode.

## Build

### Windows

Enter the evk repository `ArduCam_EVK_Demo\c\projects\async` directory in the command line

```
cmake -Bbuild .
cd build
cmake --build . --config Release
```

After compilation, you can find the demo_c_async.exe file in the `ArduCam_EVK_Demo\c\projects\async\build\Release` directory and open it in the current directory cmd, and execute
```
.\demo_c_async.exe <configuration file>
```


### Linux

```
cd c/projects/async
mkdir -p build && cd build && cmake ..
make -j

./demo_c_async <path/config-file-name>
```
