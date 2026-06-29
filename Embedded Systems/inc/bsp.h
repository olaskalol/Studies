#ifndef DONICZKA_BSP_H
#define DONICZKA_BSP_H

#include "LPC17xx.h"
#include "Common_Definitions.h"

/*!
 * @brief    Główna funkcja inicjalizująca sprzęt. 
 * Konfiguruje i uruchamia zegar systemowy, sprzętowy Timer 1 
 * oraz główne magistrale komunikacyjne (I2C2 oraz SPI/SSP1).
 *
 * @param    void
 * @returns  brak
 *
 * @side effects Zatrzymuje system w nieskończonej pętli, jeśli konfiguracja 
 * zegara SysTick się nie powiedzie. Modyfikuje rejestry kontrolne 
 * mikrokontrolera (m.in. CLKPWR, PINSEL) oraz włącza przerwania 
 * dla Timera 1 (NVIC).
 */
void bsp_init(void);

#endif