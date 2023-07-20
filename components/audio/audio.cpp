#include "audio.h"

static const char *LOG_TAG = "audio";

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
/**
 * @brief Audio 初始化
 *
 */
void Audio::init()
{
    _volume = 30;

    uart_config_t uart_structure;
    uart_structure.baud_rate = 9600;
    uart_structure.data_bits = UART_DATA_8_BITS;
    uart_structure.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_structure.parity = UART_PARITY_DISABLE;
    uart_structure.source_clk = UART_SCLK_DEFAULT;
    uart_structure.stop_bits = UART_STOP_BITS_1;
    // uart_structure.rx_flow_ctrl_thresh = 0;
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_structure);
    uart_set_pin(UART_NUM_1, AUDIO_TX_PIN, AUDIO_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    this->write(toSDcard_cmd);
    this->write(volumeSet_cmd);
    this->write(singlePlay_cmd);
    ESP_LOGI(LOG_TAG, "Initialization complete.");
}
/**
 * @brief 通过串口向JQ8900S发送命令
 *
 * @param data
 */
void Audio::write(const uint8_t *data) const
{
    uart_write_bytes(_port, data, strlen((const char *)data));
}
/**
 * @brief 计算ADD8检验和
 *
 * @param data
 * @param length
 * @return uint8_t
 */
uint8_t Audio::getADD8Check(const uint8_t *data, int length) const
{
    uint8_t checksum = 0;
    for (int i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

/**
 * @brief 根据object发送查询命令
 *
 * @param object
 * (0)所有文件数量
 * (1)当前目录文件数量
 * (2)当前文件名
 */
void Audio::query(uint8_t object) const
{
    if (object == 0)
        this->write(allfileNumQuery_cmd);
    else if (object == 1)
        this->write(fileNumQuery_cmd);
    else if (object == 2)
        this->write(fileNameQuery_cmd);
}
/**
 * @brief 根据type设置播放模式
 *
 * @param type
 *(0)单曲播放
 *(1)随机播放
 *(2)循环播放
 */
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
void Audio::pathPlay(const char *path) const
{
    int length = strlen(path);
    uint8_t cmd[length + 5];

    cmd[0] = 0xAA;
    cmd[1] = 0x08;
    cmd[2] = length + 1;
    cmd[3] = 0x01;

    for (size_t i = 0; i < length; i++)
        cmd[i + 4] = path[i];

    cmd[length + 4] = getADD8Check(cmd, length + 4);

    for (auto &&i : cmd)
    {
        printf("0x%02x ", i);
    }

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