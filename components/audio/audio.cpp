#include "audio.h"

const char *LOG_TAG = "LOG";

uint8_t calculateADD8Check(const uint8_t *data, int length)
{
    uint8_t checksum = 0;
    for (int i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

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

void Audio::init()
{
    _volume = 30;
    this->write(toSDcard_cmd);
    this->write(volumeSet_cmd);
    this->write(singlePlay_cmd);
    ESP_LOGI(LOG_TAG, "HELLO WORLD");
}

void Audio::write(const uint8_t *data) const
{
    //    int length = sizeof(data) / sizeof(data[0]);
    uart_write_bytes(_port, data, strlen((const char *)data));
}

/*object:
 0-所有文件数量
 1-当前目录下文件数量
 2-当前文件名*/
void Audio::query(uint8_t object) const
{
    if (object == 0)
        this->write(allfileNumQuery_cmd);
    else if (object == 1)
        this->write(fileNumQuery_cmd);
    else if (object == 2)
        this->write(fileNameQuery_cmd);
}
/*type:
 0-单曲播放
 1-随机播放
 2-循环播放*/
void Audio::setPalyMode(uint8_t type) const
{
    if (type == 0)
        this->write(singlePlay_cmd);
    else if (type == 1)
        this->write(randomPlay_cmd);
    else if (type == 2)
        this->write(loopPlay_cmd);
}
// 路径播放
void Audio::pathPlay(const char *path)
{
    int length = strlen(path);
    uint8_t cmd[length + 5];

    cmd[0] = 0xAA;
    cmd[1] = 0x08;
    cmd[2] = 0x04;
    cmd[3] = 0x01;

    for (size_t i = 0; i < length; i++)
    {
        cmd[i + 4] = path[i];
    }

    cmd[length + 4] = calculateADD8Check(cmd, length + 4);

    this->write(cmd);
}
// 开始
void Audio::start() const
{
    this->write(startPlay_cmd);
}
// 停止
void Audio::stop() const
{
    this->write(stopPlay_cmd);
}
// 暂停
void Audio::pause() const
{
    this->write(pausePlay_cmd);
}
// 下一曲
void Audio::next() const
{
    this->write(nextPlay_cmd);
}
// 上一曲
void Audio::previous() const
{
    this->write(previous_cmd);
}
// 设定音量
void Audio::setVolume(uint8_t volume)
{
    _volume = volume;
}
// 获取音量
uint8_t Audio::getVolume() const
{
    return _volume;
}
// 增加音量
uint8_t Audio::improveVolume()
{
    if (_volume < 30)
        _volume++;
    return _volume;
}
// 减小音量
uint8_t Audio::reduceVolume()
{
    if (_volume > 0)
        _volume--;
    return _volume;
}