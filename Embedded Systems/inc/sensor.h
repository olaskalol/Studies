#ifndef DONICZKA_SENSOR_H
#define DONICZKA_SENSOR_H

#include <stdint.h>

/*!
 * @brief    Inicjalizuje przetwornik ADC do odczytu z potencjometru.
 * @param    void
 * @returns  brak
 * @side effects Zmienia konfigurację pinów na funkcję analogową i włącza kanał ADC.
 */
void sensor_init(void);

/*!
 * @brief    Uruchamia konwersję ADC i zwraca znormalizowany poziom symulacji wilgotności.
 * @param    void
 * @returns  Znormalizowana wartość z przedziału od 1 do 16.
 * @side effects Aktywnie blokuje wykonanie programu do momentu zakończenia konwersji ADC.
 */
uint8_t sensor_get_level(void);

#endif //DONICZKA_SENSOR_H
