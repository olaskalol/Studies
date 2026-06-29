#include "../inc/led_controller.h"
#include "../inc/Common_Definitions.h"
#include "pca9532.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

static void init_LED_port_and_pin (uint8_t port, uint8_t pin)
{
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 0U;
    PinCfg.OpenDrain = 0U;
    PinCfg.Pinmode = 0U;
    PinCfg.Pinnum = pin;
    PinCfg.Portnum = port;
    PINSEL_ConfigPin (&PinCfg);
    
    GPIO_SetDir (port, (1UL << pin), 1U);
    GPIO_SetValue (port, (1UL << pin));
}

void led_controller_init(void) {
    pca9532_init();

    init_LED_port_and_pin (LED_RED_PORT, LED_RED_PIN);
    init_LED_port_and_pin (LED_BLUE_PORT, LED_BLUE_PIN);
    init_LED_port_and_pin (LED_GREEN_PORT, LED_GREEN_PIN);

    init_LED_port_and_pin (LED_RED_PORT_ALT, LED_RED_PIN_ALT);
    init_LED_port_and_pin (LED_BLUE_PORT_ALT, LED_BLUE_PIN_ALT);
    init_LED_port_and_pin (LED_GREEN_PORT_ALT, LED_GREEN_PIN_ALT);
}

void led_controller_update_moisture(uint8_t current_level) {
    uint16_t ledOn = 0U;
    uint8_t i = 0U;

    if (current_level <= 8U) {
        uint8_t numCzerwonych = (uint8_t)(9U - current_level);
        for (i = 0U; i < numCzerwonych; i++) {
            ledOn |= (uint16_t)(1UL << i);
        }
    } else {
        uint8_t numZielonych = (uint8_t)(current_level - 8U);
        for (i = 0U; i < numZielonych; i++) {
            uint32_t shift_val = (uint32_t)i + 8U;
            uint32_t mask32 = (uint32_t)1U << shift_val;
            ledOn = (uint16_t)(ledOn | (uint16_t)mask32);
        }
    }
    pca9532_setLeds(ledOn, 0xFFFFU);
}