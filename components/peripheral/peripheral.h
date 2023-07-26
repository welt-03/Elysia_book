#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <stdio.h>
#include "string.h"
#include "driver/uart.h"

// #include "esp_log.h"

class UART
{
private:
    uart_port_t _uart_num = UART_NUM_0;

public:
    int length = 0;
    uint8_t payload[50];

    UART();
    UART(uart_port_t uart_num);
    ~UART();

    void begin(int baud_rate, QueueHandle_t *uart_queue, int uart_tx_pin = UART_PIN_NO_CHANGE, int uart_rx_pin = UART_PIN_NO_CHANGE);
    void write(const void *data);
    int read();
};

#endif