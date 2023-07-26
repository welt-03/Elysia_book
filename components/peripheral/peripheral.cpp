#include "peripheral.h"

UART::UART()
{
}

UART::UART(uart_port_t uart_num)
{
    _uart_num = uart_num;
}

UART::~UART()
{
    uart_driver_delete(_uart_num);
}

void UART::begin(int baud_rate, QueueHandle_t *uart_queue, int uart_tx_pin, int uart_rx_pin)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_param_config(_uart_num, &uart_config);
    if ((uart_tx_pin != UART_PIN_NO_CHANGE) && (uart_rx_pin != UART_PIN_NO_CHANGE))
    {
        uart_set_pin(_uart_num, uart_tx_pin, uart_rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    }
    uart_driver_install(_uart_num, 1024 * 2, 1024 * 2, 10, uart_queue, 0);
}

void UART::write(const void *data)
{
    uart_write_bytes(_uart_num, data, strlen((const char *)data));
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