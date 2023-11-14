#include "disk.h"

static const char* TAG = "disk";

static uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, EDPT_MSC_OUT, EDPT_MSC_IN, TUD_OPT_HIGH_SPEED ? 512 : 64),
};

static tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A,  // This is Espressif VID. This needs to be changed according to Users / Customers
    .idProduct = 0x4002,
    .bcdDevice = 0x100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01};

/**
 * @brief 对磁盘初始化并挂载文件系统
 *
 */
static esp_err_t disk_vfs_mount(sdmmc_card_t* sdcard) {
    esp_err_t ret = ESP_OK;

    ESP_LOGI(TAG, "Initializing SD card.");
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};

    const char* mount_point = BASE_PATH;

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;

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

    if (esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &sdcard) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem!");
        return ESP_FAIL;
    }

    sdmmc_card_print_info(stdout, sdcard);
    return ret;
}

static esp_err_t disk_virtual_init(sdmmc_card_t* sdcard) {
    esp_err_t ret = ESP_OK;
    bool host_init = false;

    ESP_LOGI(TAG, "Initializing SDCard");

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    slot_config.clk = SD_CLK;
    slot_config.cmd = SD_CMD;
    slot_config.d0 = SD_DATA0;
    slot_config.d1 = SD_DATA1;
    slot_config.d2 = SD_DATA2;
    slot_config.d3 = SD_DATA3;
    slot_config.width = 4;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    // not using ff_memalloc here, as allocation in internal RAM is preferred
    sdcard = (sdmmc_card_t*)malloc(sizeof(sdmmc_card_t));
    ESP_GOTO_ON_FALSE(sdcard, ESP_ERR_NO_MEM, clean, TAG, "could not allocate new sdmmc_card_t");

    ESP_GOTO_ON_ERROR((*host.init)(), clean, TAG, "Host Config Init fail");
    host_init = true;

    ESP_GOTO_ON_ERROR(sdmmc_host_init_slot(host.slot, (const sdmmc_slot_config_t*)&slot_config),
                      clean, TAG, "Host init slot fail");

    while (sdmmc_card_init(&host, sdcard)) {
        ESP_LOGE(TAG, "The detection pin of the slot is disconnected(Insert uSD card). Retrying...");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, sdcard);

    ret = tinyusb_msc_storage_mount(BASE_PATH);

    return ret;

clean:
    if (host_init) {
        if (host.flags & SDMMC_HOST_FLAG_DEINIT_ARG) {
            host.deinit_p(host.slot);
        } else {
            (*host.deinit)();
        }
    }
    return ret;
}
