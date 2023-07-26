#include "audio.h"

static const char *LOG_TAG = "audio";

Audio::Audio()
{
}

Audio::Audio(uart_port_t uart_port)
{
    _uart_port = uart_port;
}

Audio::~Audio()
{
    uart_driver_delete(_uart_port);
}
/**
 * @brief Audio 初始化
 *
 */
void Audio::begin(QueueHandle_t *uart_queue, int baud_rate, int uart_tx_pin, int uart_rx_pin)
{
    _volume = 30;

    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT};

    uart_param_config(_uart_port, &uart_config);
    if ((uart_tx_pin != UART_PIN_NO_CHANGE) && (uart_rx_pin != UART_PIN_NO_CHANGE))
    {
        uart_set_pin(_uart_port, uart_tx_pin, uart_rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    }
    uart_driver_install(_uart_port, BUF_SIZE * 2, BUF_SIZE * 2, 10, uart_queue, 0);

    write(toSDcard_cmd, 5);
    vTaskDelay(10);
    write(volumeSet_cmd, 4);
    vTaskDelay(10);
    write(singlePlay_cmd, 5);
    ESP_LOGI(LOG_TAG, "Initialization complete.");
}
/**
 * @brief 通过串口向JQ8900S发送命令
 *
 * @param data
 */
void Audio::write(const void *data, size_t size) const
{
    uart_write_bytes(_uart_port, data, size);
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
        write(allfileNumQuery_cmd, 4);
    else if (object == 1)
        write(fileNumQuery_cmd, 4);
    else if (object == 2)
        write(fileNameQuery_cmd, 4);
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
        write(singlePlay_cmd, 5);
    else if (type == 1)
        write(randomPlay_cmd, 5);
    else if (type == 2)
        write(loopPlay_cmd, 5);
}
// 路径播放
void Audio::pathPlay(const char *path) const
{
    int length = strlen(path);
    uint8_t *cmd = (uint8_t *)malloc(length + 5);

    cmd[0] = 0xAA;
    cmd[1] = 0x08;
    cmd[2] = length + 1;
    cmd[3] = 0x01;

    for (size_t i = 0; i < length; i++)
        cmd[i + 4] = path[i];

    cmd[length + 4] = getADD8Check(cmd, length + 4);

    write(cmd, length + 5);
    free(cmd);
}
// 开始
void Audio::start() const
{
    write(startPlay_cmd, 4);
}
// 停止
void Audio::stop() const
{
    write(stopPlay_cmd, 4);
}
// 暂停
void Audio::pause() const
{
    write(pausePlay_cmd, 4);
}
// 下一曲
void Audio::next() const
{
    write(nextPlay_cmd, 4);
}
// 上一曲
void Audio::previous() const
{
    write(previous_cmd, 4);
}
// 设定音量
void Audio::setVolume(uint8_t volume)
{
    uint8_t cmd[] = {0xAA, 0x13, 0x01, 0x00, 0x00};
    _volume = volume;

    cmd[3] = _volume;
    cmd[4] = getADD8Check(cmd, 4);

    write(cmd, 5);
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
    setVolume(_volume);
    return _volume;
}
// 减小音量
uint8_t Audio::reduceVolume()
{
    if (_volume > 0)
        _volume--;
    setVolume(_volume);
    return _volume;
}