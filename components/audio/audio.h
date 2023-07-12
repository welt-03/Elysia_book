#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include "string.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "book_init.h"

class Audio
{
private:
    uint16_t _id;
    uart_port_t _port;

    const uint8_t file_query[4] = {0xAA, 0x1E, 0x00, 0xC8};
    const uint8_t number_query[4] = {0xAA, 0x12, 0x00, 0xBC};
    const uint8_t allnum_query[4] = {0xAA, 0x0C, 0x00, 0xB6};
    const uint8_t nextcomand[4] = {0xAA, 0x06, 0x00, 0xB0};
    const uint8_t previouscomand[4] = {0xAA, 0x05, 0x00, 0xAF};
    const uint8_t outputDeviceSDcardcomand[5] = {0xAA, 0x0B, 0x01, 0x01, 0xB7};
    const uint8_t startcomand[4] = {0xAA, 0x02, 0x00, 0xAC};
    const uint8_t pausecomand[4] = {0xAA, 0x03, 0x00, 0xAD};
    const uint8_t stopcomand[4] = {0xAA, 0x04, 0x00, 0xAE};
    const uint8_t randomAllcomand[5] = {0xAA, 0x18, 0x01, 0x05, 0xC8};
    const uint8_t enableLoopcomand[5] = {0xAA, 0x18, 0x01, 0x04, 0xC7};
    const uint8_t disableLoopcomand[5] = {0xAA, 0x18, 0x01, 0x02, 0xC5};

public:
    Audio();
    Audio(uart_port_t port);
    ~Audio();

    void write(const char *data);
    uint16_t query_fileNumber();
};

#endif