#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include "string.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "book_init.h"
#include "esp_log.h"

class Audio
{
private:
    uint8_t _volume;
    uart_port_t _port;

    const char *LOG_TAG = "Audio";
    const uint8_t fileNameQuery_cmd[4] = {0xAA, 0x1E, 0x00, 0xC8};
    const uint8_t fileNumQuery_cmd[4] = {0xAA, 0x12, 0x00, 0xBC};
    const uint8_t allfileNumQuery_cmd[4] = {0xAA, 0x0C, 0x00, 0xB6};
    const uint8_t volumeSet_cmd[5] = {0xAA, 0x13, 0x01, 0x1E, 0xDC};
    const uint8_t nextPlay_cmd[4] = {0xAA, 0x06, 0x00, 0xB0};
    const uint8_t previous_cmd[4] = {0xAA, 0x05, 0x00, 0xAF};
    const uint8_t toSDcard_cmd[5] = {0xAA, 0x0B, 0x01, 0x01, 0xB7};
    const uint8_t startPlay_cmd[4] = {0xAA, 0x02, 0x00, 0xAC};
    const uint8_t pausePlay_cmd[4] = {0xAA, 0x03, 0x00, 0xAD};
    const uint8_t stopPlay_cmd[4] = {0xAA, 0x04, 0x00, 0xAE};
    const uint8_t randomPlay_cmd[5] = {0xAA, 0x18, 0x01, 0x05, 0xC8};
    const uint8_t loopPlay_cmd[5] = {0xAA, 0x18, 0x01, 0x04, 0xC7};
    const uint8_t singlePlay_cmd[5] = {0xAA, 0x18, 0x01, 0x02, 0xC5};

public:
    Audio();
    Audio(uart_port_t port);
    ~Audio();

    void init();
    void write(const uint8_t *data) const;
    void query(uint8_t object) const;
    void setPalyMode(uint8_t type) const;
    void pathPlay(const char *path) const;
    void start() const;
    void stop() const;
    void pause() const;
    void next() const;
    void previous() const;
    void setVolume(uint8_t volume);

    uint8_t getADD8Check(const uint8_t *data, int length) const;
    uint8_t getVolume() const;
    uint8_t improveVolume();
    uint8_t reduceVolume();
};

#endif