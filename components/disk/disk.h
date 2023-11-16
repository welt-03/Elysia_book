#pragma once

#include <dirent.h>
#include "errno.h"
#include "esp_check.h"
#include "esp_vfs_fat.h"

#include "diskio_impl.h"
#include "diskio_sdmmc.h"
#include "tinyusb.h"
#include "tusb_msc_storage.h"

#define BASE_PATH "/sdcard"

#define SD_CLK GPIO_NUM_36
#define SD_CMD GPIO_NUM_35
#define SD_DATA0 GPIO_NUM_37
#define SD_DATA1 GPIO_NUM_38
#define SD_DATA2 GPIO_NUM_33
#define SD_DATA3 GPIO_NUM_34

#ifdef __cplusplus
extern "C" {
#endif
esp_err_t disk_vfs_mount(sdmmc_card_t* card);
esp_err_t disk_virtual_init(sdmmc_card_t** card);
#ifdef __cplusplus
}
#endif