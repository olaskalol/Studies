#ifndef DONICZKA_LED_CONTROLLER_H
#define DONICZKA_LED_CONTROLLER_H

#include <stdint.h>

/*!
 * @brief    Inicjalizuje ekspander portów PCA9532 oraz piny sterujące paskami LED.
 * @param    void
 * @returns  brak
 * @side effects Konfiguruje piny GPIO oraz nawiązuje komunikację I2C z ekspanderem.
 */
void led_controller_init(void);

/*!
 * @brief                   Wysterowuje diody LED na podstawie poziomu wilgotności gleby.
 * @param current_level     Znormalizowany poziom wilgotności.
 * @returns                 brak
 * @side effects Wysyła komendy po szynie I2C do ekspandera PCA9532, zmieniając stan świecenia diod.
 */
void led_controller_update_moisture(uint8_t current_level);

#endif //DONICZKA_LED_CONTROLLER_H
