#include <stdbool.h>
#include <arducam/arducam_evk_sdk.h>
#include <stdio.h>
#include <stdlib.h>

void list_devices() {
    ArducamDeviceListHandle devs;
    ArducamListDevice(&devs);
    printf("Total %d devices.\n", devs->size);
    for (int index = 0; index < devs->size; index++) {
        printf("%d: [%04x:%04x]\n", index + 1, devs->devices[index]->id_vendor, devs->devices[index]->id_product);
    }
}
