#include "../inc/temperature.h"
#include <stddef.h>
#include "../inc/ui.h"
#include "temp.h"

static uint32_t msTicks = 0U;

void SysTick_Handler(void) {
    msTicks++;
}

uint32_t getTicks(void) {
    return msTicks;
}

void temperature_init(void) {
    temp_init(&getTicks);
}

const uint8_t* read_temperature(void) {
    static uint8_t buf[16];
    int32_t t = temp_read();
    int c = t / 10;
    int u = t % 10;
    
    if (u < 0) {
        u = -u;
    }

    intToString(c, buf, 16U, 10U);
    uint32_t i = 0U;
    
    while ((i < 7U) && (buf[i] != (uint8_t)'\0')) {
        i++;
    }

    buf[i] = (uint8_t)'.';
    i++;
    buf[i] = (uint8_t)('0' + u);
    i++;
    buf[i] = (uint8_t)' ';
    i++;
    buf[i] = (uint8_t)'C';
    i++;
    buf[i] = (uint8_t)'\0';

    return buf;
}
