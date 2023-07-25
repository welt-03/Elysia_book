#include "peripheral.h"

UART::UART()
{
    uart_driver_install(_uart_num, 1024 * 2, 1024 * 2, 0, NULL, 0);
}

UART::UART(uint8_t uart_num)
{
    _uart_num = uart_num;
    UART();
}

UART::~UART()
{
    uart_driver_delete(_uart_num);
}

void UART::begin(int baud_rate, int uart_tx_pin, int uart_rx_pin)
{
    uart_config_t uart_config;

    uart_config.baud_rate = baud_rate;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;
    uart_config.stop_bits = UART_STOP_BITS_1;

    uart_param_config(_uart_num, &uart_config);
    if ((uart_tx_pin != UART_PIN_NO_CHANGE) && (uart_rx_pin != UART_PIN_NO_CHANGE))
    {
        uart_set_pin(_uart_num, uart_tx_pin, uart_rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    }
}

void UART::write(const char *data)
{
    uart_write_bytes(_uart_num, data, strlen(data));
}

int UART::read()
{
    uart_get_buffered_data_len(_uart_num, (size_t *)&length);
    if (length)
    {
        uart_read_bytes(_uart_num, payload, length, 0);
        uart_flush_input(_uart_num);
        return length;
    }
    return 0;
}