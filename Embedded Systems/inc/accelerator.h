#ifndef DONICZKA_ACCELERATOR_H
#define DONICZKA_ACCELERATOR_H

#include <stdint.h>
#include "Common_Definitions.h"

/*!
 * @brief    Inicjalizuje akcelerometr i pobiera początkowe wartości referencyjne.
 * @param    void
 * @returns  brak
 * @side effects Wywołuje sprzętową inicjalizację układu czujnika. 
 * Modyfikuje statyczne zmienne modułu przechowujące bieżące odczyty 
 * oraz obliczone na ich podstawie offsety kalibracyjne.
 */
void accelerator_init(void);

/*!
 * @brief    Odczytuje bieżący stan akcelerometru i sprawdza, czy płytka jest przechylona.
 * @param    void
 * @returns  Wskaźnik do statycznego ciągu znaków (np. "stable" lub "tilted").
 * @side effects Zwraca wskaźnik na lokalny, statyczny bufor, co oznacza, że kolejne wywołania nadpiszą poprzedni wynik.
 */
uint8_t* read_accelerator(void);

#endif /* DONICZKA_ACCELERATOR_H */
