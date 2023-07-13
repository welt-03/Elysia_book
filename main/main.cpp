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

extern "C" void app_main(void)
{
	book_init();
	audio.init();

	while (true)
	{
		vTaskDelay(1000);
	}
}