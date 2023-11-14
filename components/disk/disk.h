#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include "esp_log.h"
#include "errno.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

#include "tinyusb.h"
#include "tusb_msc_storage.h"
#include "diskio_sdmmc.h"
#include "diskio_impl.h"

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
static void disk_vfs_mount(void);
static esp_err_t disk_virtual_init(sdmmc_card_t **card);
static void _mount(void);
#ifdef __cplusplus
}
#endif