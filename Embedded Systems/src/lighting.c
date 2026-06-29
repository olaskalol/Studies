#include "../inc/lighting.h"
#include <stddef.h>
#include "../inc/ui.h"
#include "light.h"

void lighting_init(void) {
    light_init();
    light_enable();
    light_setRange(LIGHT_RANGE_4000);
}

const uint8_t* read_light(void) {
    static uint8_t buf[16];
    uint32_t lux = light_read();
    intToString((int)lux, buf, 16U, 10U);
    return buf;
}