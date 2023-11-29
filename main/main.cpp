#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "book_config.h"
#include "disk.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_lvgl_port.h"

#define LED_GREEN GPIO_NUM_15
#define LED_YELLOW GPIO_NUM_16

#define BUTTON_OK GPIO_NUM_0
#define BUTTON_DW GPIO_NUM_11
#define BUTTON_UP GPIO_NUM_10
#define BUTTON_MENU GPIO_NUM_14

#define BUF_SIZE 1024

static const char* TAG = "main";

//static sdmmc_card_t *mount_card = NULL;
//const char *disk_path = "/disk";

 static QueueHandle_t gpio_evt_queue = NULL;

struct BOOK_INFO {
    uint8_t id;
    uint8_t mode;
    book_state_t state;
} Elysia = {.id = 0, .mode = 0, .state = USUAL};

 static void gpio_task_handler(void* arg) {
     uint32_t io_num;
     while (true) {
         if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
             ESP_LOGI(TAG, "GPIO[%" PRIu32 "] intr, val: %d", io_num, gpio_get_level((gpio_num_t)io_num));
             switch (io_num) {
                 case BUTTON_OK:
                     break;
                 case BUTTON_DW:
                     gpio_set_level(LED_GREEN, !gpio_get_level((gpio_num_t)io_num));
                     break;
                 case BUTTON_UP:
                     gpio_set_level(LED_YELLOW, !gpio_get_level((gpio_num_t)io_num));
                     break;
                 case BUTTON_MENU:
                     break;
                 default:
                     ESP_LOGE(TAG, "read timeout.");
                     break;
             }
             gpio_intr_enable((gpio_num_t)io_num);
         }
     }
 }

/**
 * @brief 中断处理函数
 *
 * @param arg
 */
 static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t io_num = (uint32_t)arg;

    gpio_intr_disable((gpio_num_t)io_num);
    xQueueSendFromISR(gpio_evt_queue, &io_num, NULL);
 }

extern "C" void app_main(void) {
     gpio_config_t io_conf = {
         .pin_bit_mask = BIT64(BUTTON_OK) | BIT64(BUTTON_DW) | BIT64(BUTTON_UP) | BIT64(BUTTON_MENU),
         .mode = GPIO_MODE_INPUT,
         .pull_up_en = GPIO_PULLUP_ENABLE,
         .pull_down_en = GPIO_PULLDOWN_DISABLE,
         .intr_type = GPIO_INTR_ANYEDGE,
     };
     gpio_config(&io_conf);
     io_conf.pin_bit_mask = BIT64(LED_GREEN) | BIT64(LED_YELLOW);
     io_conf.mode = GPIO_MODE_OUTPUT;
     io_conf.intr_type = GPIO_INTR_DISABLE;
     gpio_config(&io_conf);
     gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
     xTaskCreate(gpio_task_handler, "gpio_task", BUF_SIZE * 2, NULL, 10, NULL);

     gpio_install_isr_service(0);

     gpio_isr_handler_add(BUTTON_DW, gpio_isr_handler, (void*)BUTTON_DW);
     gpio_isr_handler_add(BUTTON_UP, gpio_isr_handler, (void*)BUTTON_UP);
     gpio_isr_handler_add(BUTTON_MENU, gpio_isr_handler, (void*)BUTTON_MENU);
     gpio_isr_handler_add(BUTTON_OK, gpio_isr_handler, (void*)BUTTON_OK);

    ESP_LOGI(TAG, "GPIO isr init complete.");


}