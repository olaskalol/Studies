#include "../inc/blinking_led.h"
#include "lpc17xx_gpio.h"
#include "../inc/bsp.h" 

#define LED_PIN (1UL << 22U)
#define LED_PORT 0U

void blinking_led_init(void) {
    GPIO_SetDir(LED_PORT, LED_PIN, 1U);
    GPIO_SetValue(LED_PORT, LED_PIN);
}

void blinking_led_update(Bool is_active, uint32_t current_tick) {
    if (is_active == TRUE) {
        if (((current_tick / 200U) % 2U) == 0U) {
            GPIO_ClearValue(LED_PORT, LED_PIN);
        } else {
            GPIO_SetValue(LED_PORT, LED_PIN);
        }
    } else {
        GPIO_SetValue(LED_PORT, LED_PIN);
    }
}