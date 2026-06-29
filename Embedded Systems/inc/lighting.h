#ifndef DONICZKA_LIGHTING_H
#define DONICZKA_LIGHTING_H

#include <stdint.h>
#include "Common_Definitions.h"

/*!
 * @brief    Inicjalizuje czujnik natężenia światła i ustawia jego domyślny zakres pracy.
 * @param    void
 * @returns  brak
 * @side effects Włącza zasilanie/komunikację z czujnikiem po szynie I2C.
 */
void lighting_init(void);

/*!
 * @brief    Odczytuje wartość natężenia światła i formatuje ją jako ciąg znaków.
 * @param    void
 * @returns  Wskaźnik do statycznego ciągu znaków reprezentującego wartość w luksach.
 * @side effects Zwraca wskaźnik na lokalny, statyczny bufor, co oznacza, że kolejne wywołania nadpiszą poprzedni wynik.
 */
const uint8_t* read_light(void);

#endif /* DONICZKA_LIGHTING_H */
