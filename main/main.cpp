#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "book_config.h"
#include "audio.h"

const char *LOG_TAG = "main";

uint8_t usb_mode = 0;

Audio audio(UART_NUM_1);

void usb_toggle_handler(void *parameter)
{
	uint8_t _usb_mode = 0;
	// 任务代码
	vTaskDelay(20 / portTICK_PERIOD_MS);

	_usb_mode = gpio_get_level(CC_TEST) << 4;
	_usb_mode |= gpio_get_level(USB_PIN);

	if (_usb_mode != usb_mode)
	{
		if ((_usb_mode & 0xf0) != (usb_mode & 0xf0))
		{
			if (!(_usb_mode & 0xf0))
			{
				ESP_LOGI(LOG_TAG, "USB->Audio");
				gpio_set_level(ASW_IN0, HIGH);
				gpio_set_level(ASW_IN1, HIGH);
			}
			else
			{
				if (_usb_mode & 0x0f)
				{
					ESP_LOGI(LOG_TAG, "USB->SDcard");
					gpio_set_level(ASW_IN0, HIGH);
					gpio_set_level(ASW_IN1, LOW);
				}
				else
				{
					ESP_LOGI(LOG_TAG, "USB->UART");
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
				gpio_set_level(ASW_IN0, HIGH);
				gpio_set_level(ASW_IN1, LOW);
			}
			else
			{
				ESP_LOGI(LOG_TAG, "USB->UART");
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

void IRAM_ATTR usbToggleIntrHandler(void *arg)
{
	// 禁用中断
	gpio_intr_disable(USB_PIN);
	gpio_intr_disable(CC_TEST);
	// 创建任务
	xTaskCreate(usb_toggle_handler, "USB_Task", 2048, NULL, 1, NULL);
}

extern "C" void app_main(void)
{
	book_init();
	audio.init();

	xTaskCreate(usb_toggle_handler, "USB_Task", 2048, NULL, 1, NULL);

	gpio_install_isr_service(0);
	gpio_isr_handler_add(USB_PIN, usbToggleIntrHandler, NULL);
	gpio_isr_handler_add(CC_TEST, usbToggleIntrHandler, NULL);
	// audio.pathPlay("/INSERT*???");
	audio.start();

	while (true)
	{
		vTaskDelay(1000);
	}
}