#include "../inc/bsp.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_dac.h"

void bsp_init(void) {

    if (SysTick_Config(SystemCoreClock / 1000U) != 0U) {
        for (;;) { 
        }
    }

    PINSEL_CFG_Type PinCfg = {0};

    // I2C
    PinCfg.Funcnum = 2U;
    PinCfg.Pinnum = 10U;
    PinCfg.Portnum = 0U;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 11U;
    PINSEL_ConfigPin(&PinCfg);
    I2C_Init(LPC_I2C2, 100000U);
    I2C_Cmd(LPC_I2C2, ENABLE);

    // SSP
    SSP_CFG_Type SSP_ConfigStruct;

	/*
	 * Initialize SPI pin connect
	 * P0.7 - SCK;
	 * P0.8 - MISO
	 * P0.9 - MOSI
	 * P2.2 - SSEL - used as GPIO
	 */
	PinCfg.Funcnum = 2U;
	PinCfg.OpenDrain = 0U;
	PinCfg.Pinmode = 0U;
	PinCfg.Portnum = 0U;
	PinCfg.Pinnum = 7U;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8U;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9U;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Funcnum = 0U;
	PinCfg.Portnum = 2U;
	PinCfg.Pinnum = 2U;
	PINSEL_ConfigPin(&PinCfg);

	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);
	SSP_Cmd(LPC_SSP1, ENABLE);
}
