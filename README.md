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

1. opencv
2. arducam-evk-sdk-dev
3. arducam-config-parser-dev

```
sudo apt install libopencv-dev

curl -s --compressed "https://arducam.github.io/arducam_ppa/KEY.gpg" | sudo apt-key add -
sudo curl -s --compressed -o /etc/apt/sources.list.d/arducam_list_files.list "https://arducam.github.io/arducam_ppa/arducam_list_files.list"
sudo apt update
sudo apt install arducam-config-parser-dev arducam-evk-sdk-dev
```

### set udev rules

[Download script](https://github.com/ArduCAM/ArduCAM_USB_Camera_Shield/releases/download/install_drivers/configure_udev_rules.sh)

![udev](img/udev.png)

execute script

```
sudo chmod +x configure_udev_rules.sh
./configure_udev_rules.sh
```
