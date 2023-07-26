#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "book_config.h"
#include "audio.h"
#include "peripheral.h"

#define BUF_SIZE 1024
const char *LOG_TAG = "main";

uint8_t usb_mode = 0;
QueueHandle_t uart_queue;

Audio audio;
UART Serial;

void usb_toggle_task(void *parameter)
{
	uint8_t _usb_mode = 0;
	// 任务代码
	vTaskDelay(50 / portTICK_PERIOD_MS);

	_usb_mode = gpio_get_level(CC_TEST) << 4;
	_usb_mode |= gpio_get_level(USB_PIN);

	if (_usb_mode != usb_mode)
	{
		if ((_usb_mode & 0xf0) != (usb_mode & 0xf0))
		{
			if (!(_usb_mode & 0xf0))
			{
				ESP_LOGI(LOG_TAG, "USB->Audio");
				gpio_set_level(AUDIO_EN_PIN, LOW);
				gpio_set_level(ASW_IN0, HIGH);
				gpio_set_level(ASW_IN1, HIGH);
			}
			else
			{
				if (_usb_mode & 0x0f)
				{
					ESP_LOGI(LOG_TAG, "USB->SDcard");
					gpio_set_level(AUDIO_EN_PIN, HIGH);
					gpio_set_level(ASW_IN0, HIGH);
					gpio_set_level(ASW_IN1, LOW);
				}
				else
				{
					ESP_LOGI(LOG_TAG, "USB->UART");
					gpio_set_level(AUDIO_EN_PIN, LOW);
					gpio_set_level(ASW_IN0, LOW);
					gpio_set_level(ASW_IN1, LOW);
				}
			}
		}
		else if ((_usb_mode & 0xf0))
		{
			if (_usb_mode & 0x0f)
			{
				ESP_LOGI(LOG_TAG, "USB->SDcard");
				gpio_set_level(AUDIO_EN_PIN, HIGH);
				gpio_set_level(ASW_IN0, HIGH);
				gpio_set_level(ASW_IN1, LOW);
			}
			else
			{
				ESP_LOGI(LOG_TAG, "USB->UART");
				gpio_set_level(AUDIO_EN_PIN, LOW);
				gpio_set_level(ASW_IN0, LOW);
				gpio_set_level(ASW_IN1, LOW);
			}
		}
		usb_mode = _usb_mode;
	}

	// 重新启用中断
	gpio_intr_enable(USB_PIN);
	gpio_intr_enable(CC_TEST);
	// 删除任务
	vTaskDelete(NULL);
}

void IRAM_ATTR usb_toggle_intr_handler(void *arg)
{
	// 禁用中断
	gpio_intr_disable(USB_PIN);
	gpio_intr_disable(CC_TEST);
	// 创建任务
	xTaskCreate(usb_toggle_task, "USB_Task", 2048, NULL, 1, NULL);
}

void uart_event_task(void *pvParameters)
{
	uart_event_t event;
	uint8_t *data_buf = (uint8_t *)malloc(BUF_SIZE);

	while (true)
	{
		if (xQueueReceive(uart_queue, (void *)&event, (TickType_t)portMAX_DELAY))
		{
			memset(data_buf, 0, BUF_SIZE);	//数据清空
			switch (event.type)
			{
			case UART_DATA:
				uart_read_bytes(UART_NUM_0, data_buf, event.size, portMAX_DELAY);
				uart_write_bytes(UART_NUM_1, data_buf, event.size);
				break;
			default:
				ESP_LOGI(LOG_TAG, "uart event type: %d", event.type);
				break;
			}
		}
	}

	free(data_buf);
	data_buf = NULL;
	vTaskDelete(NULL);
}

extern "C" void app_main(void)
{
	book_init();
	audio.begin();
	Serial.begin(115200, &uart_queue);

	xTaskCreate(usb_toggle_task, "USB_Task", 2048, NULL, 1, NULL);
	xTaskCreate(uart_event_task, "UART_Task", 2048, NULL, 10, NULL);

	gpio_install_isr_service(0);
	gpio_isr_handler_add(USB_PIN, usb_toggle_intr_handler, NULL);
	gpio_isr_handler_add(CC_TEST, usb_toggle_intr_handler, NULL);

	//audio.pathPlay("/INSERT*???");
	while (true)
	{
		vTaskDelay(1000);
	}
}