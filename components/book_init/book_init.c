#include "book_init.h"

void book_init()
{
    gpio_config_t gpio_structure;

    gpio_structure.intr_type = GPIO_INTR_NEGEDGE;
    gpio_structure.mode = GPIO_MODE_INPUT;
    gpio_structure.pin_bit_mask = BIT64(SW1_PIN) | BIT64(SW2_PIN) | BIT64(SW4_PIN);
    gpio_structure.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_structure.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&gpio_structure);
    gpio_structure.intr_type = GPIO_INTR_ANYEDGE;
    gpio_structure.pin_bit_mask = BIT64(SW3_PIN) | BIT64(CC_TEST) | BIT64(USB_PIN);
    gpio_config(&gpio_structure);

    gpio_structure.intr_type = GPIO_INTR_DISABLE;
    gpio_structure.mode = GPIO_MODE_INPUT;
    gpio_structure.pin_bit_mask = BIT64(AUDIO_BUSY_PIN) | BIT64(ADC_IN_PIN);
    gpio_structure.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_structure.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&gpio_structure);

    gpio_structure.intr_type = GPIO_INTR_DISABLE;
    gpio_structure.mode = GPIO_MODE_OUTPUT;
    gpio_structure.pin_bit_mask = BIT64(ASW_IN0) | BIT64(ASW_IN1) | BIT64(AUDIO_EN_PIN);
    gpio_structure.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_structure.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&gpio_structure);

    gpio_set_level(SW1_PIN, HIGH);
    gpio_set_level(SW2_PIN, HIGH);
    gpio_set_level(SW4_PIN, HIGH);
    gpio_set_level(SW3_PIN, HIGH);
    gpio_set_level(CC_TEST, HIGH);
    gpio_set_level(USB_PIN, HIGH);

    gpio_set_level(ASW_IN0, LOW);
    gpio_set_level(ASW_IN1, LOW);
    gpio_set_level(AUDIO_EN_PIN, LOW);

    uart_config_t uart_structure;
    uart_structure.baud_rate = 9600;
    uart_structure.data_bits = UART_DATA_8_BITS;
    uart_structure.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_structure.parity = UART_PARITY_DISABLE;
    uart_structure.source_clk = UART_SCLK_DEFAULT;
    uart_structure.stop_bits = UART_STOP_BITS_1;
    uart_structure.rx_flow_ctrl_thresh = 0;
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_structure);
    uart_set_pin(UART_NUM_1, AUDIO_TX_PIN, AUDIO_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}