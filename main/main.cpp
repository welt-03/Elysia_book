#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "book_config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define LED_GREEN GPIO_NUM_15
#define LED_YELLOW GPIO_NUM_16

#define BUTTON_OK GPIO_NUM_0
#define BUTTON_DW GPIO_NUM_11
#define BUTTON_UP GPIO_NUM_10
#define BUTTON_MENU GPIO_NUM_14

#define BUF_SIZE 1024

#define LOG_TAG "main"

static QueueHandle_t gpio_evt_queue = NULL;

struct BOOK_INFO {
    uint8_t id;
    uint8_t mode;
    book_state_t state;
} Elysia = {.id = 0, .mode = 0, .state = USUAL};

void gpio_task_handler(void* arg) {
    uint32_t io_num;
    while (true) {
        //ESP_LOGI(LOG_TAG, "正在读取gpio中断事件");
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            switch (io_num) {
                case BUTTON_OK:
                    break;
                case BUTTON_DW:
                    gpio_set_level(LED_GREEN, HIGH);
                    break;
                case BUTTON_UP:
                    gpio_set_level(LED_YELLOW, HIGH);
                    break;
                case BUTTON_MENU:
                    break;
                default:
                    break;
            }
            vTaskDelay(200 / portTICK_PERIOD_MS);
            gpio_set_level(LED_GREEN, LOW);
            gpio_set_level(LED_YELLOW, LOW);
        }
    }
}

void IRAM_ATTR gpio_isr_handler(void* arg) {
    xQueueSendFromISR(gpio_evt_queue, (void*)arg, NULL);
}

extern "C" void app_main(void) {
    gpio_install_isr_service(0);

    gpio_config_t io_conf = {
        .pin_bit_mask = BIT64(BUTTON_OK) | BIT64(BUTTON_DW) | BIT64(BUTTON_UP) | BIT64(BUTTON_MENU),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&io_conf);
    io_conf.pin_bit_mask = BIT64(LED_GREEN) | BIT64(LED_YELLOW);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task_handler, "gpio_task", 2048, NULL, 10, NULL);
    gpio_isr_handler_add(BUTTON_DW, gpio_isr_handler, (void*)BUTTON_DW);
    gpio_isr_handler_add(BUTTON_UP, gpio_isr_handler, (void*)BUTTON_UP);
    gpio_isr_handler_add(BUTTON_MENU, gpio_isr_handler, (void*)BUTTON_MENU);
    gpio_isr_handler_add(BUTTON_OK, gpio_isr_handler, (void*)BUTTON_OK);

    ESP_LOGI(LOG_TAG, "GPIO isr init complete.");

    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}