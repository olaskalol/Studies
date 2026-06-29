#ifndef DONICZKA_AUDIO_H
#define DONICZKA_AUDIO_H

#include <stdint.h>
#include "Common_Definitions.h"

/*!
 * @brief    Inicjalizuje piny, przetwornik DAC oraz przygotowuje próbki dźwiękowe.
 * @param    void
 * @returns  brak
 * @side effects Zmienia konfigurację pinów oraz włącza przetwornik cyfrowo-analogowy.
 */
void audio_init(void);

/*!
 * @brief    Uruchamia odtwarzanie ostrzegawczego sygnału dźwiękowego.
 * @param    void
 * @returns  brak
 * @side effects Włącza przerwania Timera 2 (TIMER2_IRQn). Jeśli dźwięk jest już odtwarzany, funkcja nie robi nic.
 */
void audio_play_warning(void);

void TIMER2_IRQHandler(void);

#endif //DONICZKA_AUDIO_H
