#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/ringbuf.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "tinyusb.h"
#include "tusb_msc_storage.h"

#define ROOT_PATH "/sdcard"

#define SD_CLK GPIO_NUM_36
#define SD_CMD GPIO_NUM_35
#define SD_DATA0 GPIO_NUM_37
#define SD_DATA1 GPIO_NUM_38
#define SD_DATA2 GPIO_NUM_33
#define SD_DATA3 GPIO_NUM_34

#ifdef __cplusplus
extern "C" {
#endif
    void virtual_disk_init(void);
#ifdef __cplusplus
}
#endif