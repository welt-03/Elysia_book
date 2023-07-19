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

Audio audio(UART_NUM_1);

void IRAM_ATTR usbSwitchIntrHandler(void *arg)
{
	// 禁用中断
	gpio_intr_disable(USB_PIN);

	// 创建任务
	xTaskCreate(
		[](void *parameter)
		{
			// 任务代码
			vTaskDelay(20 / portTICK_PERIOD_MS);
			if (gpio_get_level(CC_TEST))
			{

				if (gpio_get_level(USB_PIN))
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
			else
			{
				ESP_LOGI(LOG_TAG, "USB->Audio");
				gpio_set_level(ASW_IN0, HIGH);
				gpio_set_level(ASW_IN1, HIGH);
			}

			// 重新启用中断
			gpio_intr_enable(USB_PIN);

			// 删除任务
			vTaskDelete(NULL);
		},
		"USB_Task",
		2048,
		NULL,
		1,
		NULL);
}

extern "C" void app_main(void)
{
	book_init();
	audio.init();

	gpio_install_isr_service(0);
	gpio_isr_handler_add(USB_PIN, usbSwitchIntrHandler, NULL);
	gpio_isr_handler_add(CC_TEST, usbSwitchIntrHandler, NULL);
	audio.start();

	while (true)
	{
		vTaskDelay(1000);
	}
}