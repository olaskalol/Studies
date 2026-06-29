#ifndef DONICZKA_UI_H
#define DONICZKA_UI_H

#include <stdint.h>
#include "Common_Definitions.h"

typedef struct {
	uint8_t min_temp;
	uint8_t max_temp;
	uint16_t min_lux;
	uint16_t max_lux;
	uint8_t min_moisture;
	uint8_t max_moisture;
} PlantConditions;


/*!
 * @brief    Inicjalizuje wyświetlacz OLED, joystick oraz pamięć EEPROM. Wczytuje zachowane ustawienia.
 * 
 * @param    void
 * @returns  brak
 * 
 * @side effects Czyści ekran wyświetlacza oraz odczytuje dane po I2C z EEPROM.
 */
void ui_init(void);

/*!
 * @brief    Odczytuje stan joysticka i aktualizuje menu.
 * 
 * @param    void
 * @returns  brak
 * 
 * @side effects Może zapisać wybrane imię do EEPROM lub zmodyfikować oczekiwane parametry. 
 * Ustawia flagę odświeżania ekranu (bNeedsRedraw), jeśli zaszła zmiana.
 */
void ui_process_input(void);

/*!
 * @brief    			Odświeża zawartość ekranu OLED, jeśli flaga bNeedsRedraw jest ustawiona.
 * 
 * @param temperature  	Wskaźnik na łańcuch znaków z wartością temperatury.
 * @param lux          	Wskaźnik na łańcuch znaków z wartością oświetlenia.
 * @param acc          	Wskaźnik na łańcuch znaków z odczytem akcelerometru.
 * @param moisture     	Wartość liczbowa wilgotności.
 * @returns  			brak
 * 
 * @side effects Nadpisuje bufor obrazu i przesyła go do kontrolera wyświetlacza. Zeruje flagę bNeedsRedraw.
 */
void ui_display(uint8_t* temperature, uint8_t* lux, uint8_t* acc, uint8_t moisture);

/*!
 * @brief    			Sprawdza, czy odczytane parametry środowiskowe wykraczają poza ustalone granice.
 * 
 * @param temperature  	Wskaźnik na ciąg znaków.
 * @param lux          	Wskaźnik na ciąg znaków.
 * @param acc          	Wskaźnik na ciąg znaków.
 * @param moisture     	Wartość liczbowa bieżącej wilgotności.
 * @returns  			TRUE jeśli którykolwiek z parametrów przekracza normę, w przeciwnym wypadku FALSE.
 * 
 * @side effects Wykonuje rzutowanie łańcuchów znaków (atoi).
 */
Bool ui_alarm(const uint8_t* temperature, const uint8_t* lux, const uint8_t* acc, uint8_t moisture);

int custom_strcmp(const uint8_t* str1, const uint8_t* str2);
void custom_strcpy(uint8_t* dest, const uint8_t* src, uint32_t max_len);

/*!
 * @brief    		Konwertuje liczbę całkowitą na ciąg znaków w zadanym systemie liczbowym.
 *
 * @param value  	Liczba całkowita ze znakiem do przekonwertowania.
 * @param pBuf   	Wskaźnik na bufor docelowy, w którym zostanie zapisany wynik.
 * @param len    	Maksymalna długość bufora (Wraz z '\0').
 * @param base   	Podstawa systemu liczbowego. Dopuszczalny zakres: od 2 do 36.
 *
 * @returns      	brak
 *
 * @side effects Modyfikuje pamięć pod adresem pBuf. Jeśli bufor będzie za mały, wynik może zostać ucięty.
 */
void intToString(int value, uint8_t* pBuf, uint32_t len, uint32_t base);

/*!
 * @brief    	Wymusza przerysowanie ekranu OLED przy najbliższym cyklu pętli głównej.
 * 
 * @param flag 	TRUE aby wymusić odświeżenie, FALSE aby je wstrzymać.
 * @returns  	brak
 * 
 * @side effects Modyfikuje statyczną zmienną bNeedsRedraw.
 */
void setRedrawFlag(Bool flag);

/*!
 * @brief    Sprawdza, czy użytkownik przebywa obecnie w jakimkolwiek podmenu konfiguracji.
 *
 * @param    void
 * @returns  TRUE jeśli użytkownik jest w menu, FALSE jeśli jest na głównym ekranie.
 * 
 * @side effects Brak.
 */
Bool getIsMenuOpened(void);

#endif /* DONICZKA_UI_H */
