#include "../inc/audio.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

extern const unsigned char sound_8k[30700];
extern int sound_sz;

static uint32_t sound_cnt = 0U;
static Bool is_playing = FALSE;
static uint32_t sound_offset = 44U;

void audio_init(void) {
    PINSEL_CFG_Type PinCfg;

    GPIO_SetDir(0U, (1UL<<27U), 1U);
    GPIO_SetDir(0U, (1UL<<28U), 1U);
    GPIO_SetDir(2U, (1UL<<13U), 1U);
    GPIO_SetDir(0U, (1UL<<26U), 1U);

    GPIO_ClearValue(0U, (1UL<<27U));
    GPIO_ClearValue(0U, (1UL<<28U));
    GPIO_ClearValue(2U, (1UL<<13U));

    PinCfg.Funcnum = 2U;
    PinCfg.OpenDrain = 0U;
    PinCfg.Pinmode = 0U;
    PinCfg.Pinnum = 26U;
    PinCfg.Portnum = 0U;
    PINSEL_ConfigPin(&PinCfg);

    DAC_Init(LPC_DAC);

    uint32_t cnt = 0U;
    while (cnt < 1000U) {
        if ((sound_8k[cnt] == (unsigned char)'d') && 
            (sound_8k[cnt+1U] == (unsigned char)'a') &&
            (sound_8k[cnt+2U] == (unsigned char)'t') && 
            (sound_8k[cnt+3U] == (unsigned char)'a')) {
            cnt += 8U;
            sound_offset = cnt;
            break;
        }
        cnt++;
    }
}

void audio_play_warning(void) {
    if (is_playing == FALSE) {
        sound_cnt = sound_offset;
        is_playing = TRUE;

        TIM_TIMERCFG_Type TIM_ConfigStruct;
        TIM_MATCHCFG_Type TIM_MatchConfigStruct;

        TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
        TIM_ConfigStruct.PrescaleValue = 1U;
        TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &TIM_ConfigStruct);

        TIM_MatchConfigStruct.MatchChannel = 0U;
        TIM_MatchConfigStruct.IntOnMatch = TRUE;
        TIM_MatchConfigStruct.ResetOnMatch = TRUE;
        TIM_MatchConfigStruct.StopOnMatch = FALSE;
        TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
        TIM_MatchConfigStruct.MatchValue = 125U;

        TIM_ConfigMatch(LPC_TIM2, &TIM_MatchConfigStruct);

        NVIC_EnableIRQ(TIMER2_IRQn);
        TIM_Cmd(LPC_TIM2, ENABLE);
    }
}

void TIMER2_IRQHandler(void) {
    if (TIM_GetIntStatus(LPC_TIM2, TIM_MR0_INT) == SET) {
        if (sound_cnt < (uint32_t)sound_sz) {
            DAC_UpdateValue(LPC_DAC, (uint32_t)sound_8k[sound_cnt]);
            sound_cnt++;
        } else {
            is_playing = FALSE;
            TIM_Cmd(LPC_TIM2, DISABLE);
        }
        TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);
    }
}
