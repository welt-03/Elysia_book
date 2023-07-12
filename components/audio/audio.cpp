#include "audio.h"

Audio::Audio()
{
    _port = UART_NUM_1;
}

Audio::Audio(uart_port_t port)
{
    _port = port;
}

Audio::~Audio()
{
}

void Audio::write(const char *data)
{
    uart_write_bytes(_port, data, strlen(data));
}
