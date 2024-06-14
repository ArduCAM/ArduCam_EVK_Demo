# [Linux Environmental Installation](./linux_environmental_install.md)

## Windows

### Install Driver

[Dowload Driver](https://github.com/ArduCAM/ArduCAM_USB_Camera_Shield/releases/download/install_drivers/install_USB_Camera_Drivers.zip)

![driver](../img/driver.png)

Unzip the package after the download is complete.

Double-click to execute the install_driver.bat file.

![driver](../img/install_driver.png)

### Install cmake

Please operate according to the corresponding package on the official website

[cmake Download link](https://cmake.org/download/)

### Install Visual Studio environment

Install `Build Tools for Visual Studio` or `Visual Studio`  
<https://visualstudio.microsoft.com/downloads/>

#### Note

When generating a CMake project for Visual Studio, you need to specify the appropriate generator for the version of Visual Studio you are using. The generators to use are:

- For Visual Studio 2017: `cmake -G "Visual Studio 15 2017 Win64" ...`
- For Visual Studio 2015: `cmake -G "Visual Studio 14 2015 Win64" ...` 
- For Visual Studio 2013: `cmake -G "Visual Studio 12 2013 Win64" ...`

Be sure to use the correct generator for the version of Visual Studio installed on your system. Using the wrong generator may result in errors or an incompatible project being generated.  
You may not need to specify the version number for newer Visual Studio releases(2019 and later).

### Install evk sdk

Download win_evk_sdk.zip
<https://github.com/ArduCAM/ArduCam_EVK_Demo/releases>

![evk_download](../img/evk_download.png)

After downloading, please unzip it to the desktop. If you unzip it in another directory, please remember the directory location as you will need it later.  

(1). Open `Advanced System Settings`  
![env-1](../img/env_1.png)
![env-2](../img/env_2.png)  
(2). Setting up compiler environment `arducam_evk_cpp_sdk_DIR`  
![env-3](../img/env_3.png)
![env-8](../img/arducam_evk_cpp_sdk_DIR.png)  
(3). Setting up compiler environment `arducam_evk_sdk_DIR`  
![env-3](../img/env_3.png)
![env-9](../img/arducam_evk_cpp_sdk_DIR.png)  

### Install Opencv

#### Note

When using OpenCV with Visual Studio, the compatible version of OpenCV depends on the version of Visual Studio:

- If you are using Visual Studio 2017 or Visual Studio 2015, you should download the latest version of OpenCV 4.6.0.
- If you are using Visual Studio 2019 or a later version, you can download the latest version of OpenCV.

Make sure to use an OpenCV version that is compatible with your Visual Studio installation to avoid any issues.

1. download and install ([opencv Releases](https://opencv.org/releases/))

![releases](../img/opencv-releases.png)
2. add environment variable

`Note`: all the environment directories shown below are examples. Please fill in the actual storage directory for actual operation.

(1). Open `Advanced System Settings`  
![env-1](../img/env_1.png)
![env-2](../img/env_2.png)  
(2). Setting up compiler environment `OpenCV_DIR`  
![env-3](../img/env_3.png)
![env-4](../img/env_4.png)  
(3). Set up the opencv executable file environment  
![env-5](../img/env_5.png)
![env-6](../img/env_6.png)
![env-7](../img/env_7.png)  
