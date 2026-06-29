#ifndef DONICZKA_BLINKING_LED_H
#define DONICZKA_BLINKING_LED_H

#include "Common_Definitions.h"

/*!
 * @brief    Konfiguruje pin GPIO obsługujący diodę LED jako wyjście.
 * @param    void
 * @returns  brak
 * @side effects Gasi diodę LED na starcie.
 */
void blinking_led_init(void);

/*!
 * @brief               Aktualizuje stan migającej diody alarmowej na podstawie bieżącego czasu systemowego.
 * 
 * @param is_active     Flaga logiczna określająca, czy alarm jest aktywny.
 * @param current_tick  Bieżący czas systemowy w milisekundach.
 * @returns             brak
 * 
 * @side effects Zmienia fizyczny stan pinu mikrokontrolera.
 */
void blinking_led_update(Bool is_active, uint32_t current_tick);

#endif /* DONICZKA_BLINKING_LED_H */
