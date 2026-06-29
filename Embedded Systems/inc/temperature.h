#ifndef DONICZKA_TEMPERATURE_H
#define DONICZKA_TEMPERATURE_H

#include <stdint.h>
#include "Common_Definitions.h"

/*!
 * @brief    Obsługa przerwania systemowego. Inkrementuje globalny licznik milisekund.
 * @param    void
 * @returns  brak
 * @side effects Wywoływana automatycznie przez sprzęt. Modyfikuje statyczną zmienną msTicks.
 */
void SysTick_Handler(void);

/*!
 * @brief    Zwraca aktualny czas systemowy w milisekundach od startu mikrokontrolera.
 * @param    void
 * @returns  Czas w milisekundach
 * @side effects Brak.
 */
uint32_t getTicks(void);

/*!
 * @brief    Inicjalizuje czujnik temperatury, przypisując mu funkcję pobierania czasu systemowego.
 * @param    void
 * @returns  brak
 * @side effects Komunikuje się z układem po magistrali I2C.
 */
void temperature_init(void);

/*!
 * @brief    Odczytuje bieżącą temperaturę i formatuje ją do postaci ciągu znaków.
 * @param    void
 * @returns  Wskaźnik do statycznego ciągu znaków.
 * @side effects Zwraca wskaźnik na lokalny, statyczny bufor, co oznacza, że kolejne wywołania nadpiszą poprzedni wynik.
 */
const uint8_t* read_temperature(void);

#endif /* DONICZKA_TEMPERATURE_H */
