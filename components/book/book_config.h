/**
 * @file book_init.h
 * @author welt-03
 * @brief
 * @version 0.1
 * @date 2023-07-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/dac.h"

#define HIGH 1 // 高电平
#define LOW 0  // 低电平

#define BUF_SIZE 1024

#define SW1_PIN GPIO_NUM_13 // 用于重置系统的按键
#define SW2_PIN GPIO_NUM_12 // 用于书页按下触发的按键
#define SW3_PIN GPIO_NUM_14 // 按下触发待机音的按键
#define SW4_PIN GPIO_NUM_15 // 拔剑触发的按键
#define USB_PIN GPIO_NUM_25 // 切换USB功能的滑动开关

#define CC_TEST GPIO_NUM_19
#define ASW_IN0 GPIO_NUM_21
#define ASW_IN1 GPIO_NUM_22

#define T0_PIN GPIO_NUM_4
#define T1_PIN GPIO_NUM_0
#define T2_PIN GPIO_NUM_2

#define AUDIO_TX_PIN GPIO_NUM_32
#define AUDIO_RX_PIN GPIO_NUM_33
#define AUDIO_EN_PIN GPIO_NUM_27
#define AUDIO_BUSY_PIN GPIO_NUM_9
#define AUDIO_VPP_PIN GPIO_NUM_10

#define ADC_IN_PIN GPIO_NUM_35
#define WS2812_PIN GPIO_NUM_5

    void book_init();

#ifdef __cplusplus
}
#endif