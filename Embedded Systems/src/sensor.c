#include "../inc/sensor.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

static uint32_t map_value(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
    return (((x - in_min) * (out_max - out_min)) / (in_max - in_min)) + out_min;
}

void sensor_init(void) {
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 3U;
    PinCfg.OpenDrain = 0U;
    PinCfg.Pinmode = 0U;
    PinCfg.Portnum = 1U;
    PinCfg.Pinnum = 31U; 
    PINSEL_ConfigPin(&PinCfg);

    ADC_Init(LPC_ADC, 200000U);

    PinCfg.Funcnum = 1U;
    PinCfg.Pinnum = 23U;
    PinCfg.Portnum = 0U;
    PINSEL_ConfigPin(&PinCfg);
    
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
}

uint8_t sensor_get_level(void) {
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);
    
    while (ADC_ChannelGetStatus(LPC_ADC, ADC_CHANNEL_0, ADC_DATA_DONE) == RESET) {
    }
    
    uint32_t raw_value = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
    uint32_t mapped = map_value(raw_value, 0U, 4095U, 1U, 16U);
    
    return (uint8_t)mapped;
}
