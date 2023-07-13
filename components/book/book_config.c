#include "book_config.h"

void book_init()
{
    gpio_config_t gpio_structure;

    gpio_structure.intr_type = GPIO_INTR_NEGEDGE;
    gpio_structure.mode = GPIO_MODE_INPUT;
    gpio_structure.pin_bit_mask = BIT64(SW1_PIN) | BIT64(SW2_PIN) | BIT64(SW4_PIN);
    gpio_structure.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_structure.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&gpio_structure);
    gpio_structure.intr_type = GPIO_INTR_ANYEDGE;
    gpio_structure.pin_bit_mask = BIT64(SW3_PIN) | BIT64(CC_TEST) | BIT64(USB_PIN);
    gpio_config(&gpio_structure);

    gpio_structure.intr_type = GPIO_INTR_DISABLE;
    gpio_structure.mode = GPIO_MODE_INPUT;
    gpio_structure.pin_bit_mask = BIT64(AUDIO_BUSY_PIN) | BIT64(ADC_IN_PIN);
    gpio_structure.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_structure.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&gpio_structure);

    gpio_structure.intr_type = GPIO_INTR_DISABLE;
    gpio_structure.mode = GPIO_MODE_OUTPUT;
    gpio_structure.pin_bit_mask = BIT64(ASW_IN0) | BIT64(ASW_IN1) | BIT64(AUDIO_EN_PIN);
    gpio_structure.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_structure.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&gpio_structure);

    gpio_set_level(SW1_PIN, HIGH);
    gpio_set_level(SW2_PIN, HIGH);
    gpio_set_level(SW4_PIN, HIGH);
    gpio_set_level(SW3_PIN, HIGH);
    gpio_set_level(CC_TEST, HIGH);
    gpio_set_level(USB_PIN, HIGH);

    gpio_set_level(ASW_IN0, LOW);
    gpio_set_level(ASW_IN1, LOW);
    gpio_set_level(AUDIO_EN_PIN, LOW);
}