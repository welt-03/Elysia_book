#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "book_config.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "audio.h"

#define BUF_SIZE 1024

const char *LOG_TAG = "main";

struct BOOK_INFO
{
	uint8_t id;
	uint8_t mode;
	book_state_t state;
} Elysia = {.id = 0, .mode = 0, .state = USUAL};

QueueHandle_t uart_queue;

Audio audio;

void usb_toggle_task(void *parameter)
{
	static uint8_t usb_mode = 0;
	uint8_t _usb_mode = 0, count = 0;

	for (size_t i = 0; i < 3; i++)
	{
		vTaskDelay(40 / portTICK_PERIOD_MS);

		_usb_mode = gpio_get_level(CC_TEST) << 4;
		_usb_mode |= gpio_get_level(USB_PIN);
		if (_usb_mode != usb_mode)
			count++;
	}

	if (count >= 3)
	{
		if ((_usb_mode & 0xf0) != (usb_mode & 0xf0))
		{
			if (!(_usb_mode & 0xf0))
			{
				ESP_LOGI(LOG_TAG, "USB->Audio");
				gpio_set_level(AUDIO_EN_PIN, HIGH);
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
			memset(data_buf, 0, BUF_SIZE); // 数据清空
			switch (event.type)
			{
			case UART_DATA:
				uart_read_bytes(UART_NUM_1, data_buf, event.size, portMAX_DELAY);
				for (size_t i = 0; i < event.size; i++)
				{
					printf("%.2x", data_buf[i]);
				}
				printf("\n");
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

void book_state_toggle_task(void *parameter)
{
	vTaskDelay(50 / portTICK_PERIOD_MS);

	if (!gpio_get_level(SW4_PIN) && (Elysia.state == STANDBY))
	{
		Elysia.state = FORMAL;
	}
	if (!gpio_get_level(SW3_PIN))
	{
		Elysia.state = STANDBY;
		audio.pathPlay("/INSERT*???");
	}
	else
	{
		Elysia.state = USUAL;
		audio.pathPlay("/DETACH*???");
	}

	gpio_intr_enable(SW3_PIN);
	gpio_intr_enable(SW4_PIN);
	vTaskDelete(NULL);
}

void IRAM_ATTR book_state_toggle_intr_handler(void *arg)
{
	// 禁用中断
	gpio_intr_disable(SW3_PIN);
	gpio_intr_disable(SW4_PIN);
	// 创建任务
	xTaskCreate(book_state_toggle_task, "BOOK_Task", 2048, NULL, 1, NULL);
}

extern "C" void app_main(void)
{
	book_init();
	audio.begin(&uart_queue);

	xTaskCreate(usb_toggle_task, "USB_Task", 2048, NULL, 1, NULL);
	xTaskCreate(uart_event_task, "UART_Task", 2048, NULL, 10, NULL);

	vTaskDelay(100 / portTICK_PERIOD_MS);
	audio.query(ALL_FILE_NUM);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	audio.query(NOW_FILE_NUM);

	gpio_install_isr_service(0);
	gpio_isr_handler_add(USB_PIN, usb_toggle_intr_handler, NULL);
	gpio_isr_handler_add(CC_TEST, usb_toggle_intr_handler, NULL);

	gpio_isr_handler_add(SW3_PIN, book_state_toggle_intr_handler, NULL);
	gpio_isr_handler_add(SW4_PIN, book_state_toggle_intr_handler, NULL);

	while (true)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}