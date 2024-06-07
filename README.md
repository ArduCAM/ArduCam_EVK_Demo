# Introduction

This is the new SDK for Arducam EVK, currently in release version v1.0.0.  

**If you want a beta version, please switch to the beta branch. You can also find an archive of beta versions on the release page, including a pdf version of the API documentation.** 

There are some differences between the release version and the beta version. If you want to migrate from the beta version to the release version, please refer to: [MigrationGuide](
https://www.arducam.com/docs/arducam-evk/Start/MigrationGuide.html#update-to-v1-0-0-from-beta-versions)

**If you want to switch to the release version, please make sure that the C/C++ beta version deb has been uninstalled: `sudo apt purge arducam-evk-sdk-beta-dev`**

## API documentation

For detailed API reference and usage instructions,
please refer to the [Arducam online documentation](https://www.arducam.com/docs/arducam-evk/).

## Installation dependence
### Linux
please refer to the [Linux installation](doc/linux_environmental_install.md)

### Windows
please refer to the [Windows installation](doc/windows_environmental_install.md)

## What's next
The project contains multiple demos, please try them according to the corresponding descriptions.
```
    .
    ├── c                           # C Demo
    │   ├── COption.cmake           # C Demo CMake compilation parameter settings and import
    │   ├── functions               # C Demo source code (independent use cases for each function)
    │   └── projects                # C Demo project (integration use cases of common functions)
    │       ├── FindOpenCV.cmake    # CMake looking for OpenCV library
    │       ├── simple              # Simple synchronous capture demo
    │       ├── simple-async        # Simple asynchronous capture demo
    │       ├── sync                # Synchronous capture demo
    │       └── async               # Asynchronous capture demo
    ├── c++                         # C++ Demo
    │   ├── CppOption.cmake         # C++ Demo CMake compile parameter settings and import arducam_evk_cpp_sdk
    │   ├── functions               # C++ Demo source code (independent use cases for each function)
    │   └── projects                # C++ Demo project (integration use cases of common functions)
    │       ├── FindOpenCV.cmake    # CMake looking for OpenCV library
    │       ├── sync                # Synchronous capture demo
    │       └── async               # Asynchronous capture demo
    ├── python                      # Python Demo
    │   ├── requirements.txt        # python demo dependent libraries
    │   ├── README.md               # python demo introduction and usage
    │   ├── function                # python demo source code (independent use cases for each function)
    │   └── project                 # python demo project (integrated use cases for common functions)
    │       ├── utils.py            # Project tool function
    │       ├── sync.py             # Synchronous capture demo
    │       └── async.py            # Asynchronous capture demo
    └── README.md                   
```