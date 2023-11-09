#include "disk.h"

static const char* TAG = "disk";

void virtual_disk_init(void) {
    ESP_LOGI(TAG, "Initializing SD card");
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};

    const char* mount_point = ROOT_PATH;
    sdmmc_card_t* sdcard;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    host.flags &= SDMMC_HOST_FLAG_DEINIT_ARG;
    host.deinit_p(host.slot);

    // 配置SD卡检测并配置引脚
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    slot_config.clk = SD_CLK;
    slot_config.cmd = SD_CMD;
    slot_config.d0 = SD_DATA0;
    slot_config.d1 = SD_DATA1;
    slot_config.d2 = SD_DATA2;
    slot_config.d3 = SD_DATA3;
    slot_config.width = 4;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    sdcard = (sdmmc_card_t*)malloc(sizeof(sdmmc_card_t));
    if (sdmmc_host_init_slot(host.slot, &slot_config) != ESP_OK)
        ESP_LOGE(TAG, "Failed to init host!");
    while (sdmmc_card_init(&host, sdcard) != ESP_OK) {
        ESP_LOGE(TAG, "The detection pin of the slot is disconnected(Insert uSD card). Retrying...");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

    //if (esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &sdcard) != ESP_OK)
    //    ESP_LOGE(TAG, "Failed to mount filesystem!");

    sdmmc_card_print_info(stdout, sdcard);
    free(sdcard);
}
