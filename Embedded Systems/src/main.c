#include "../inc/bsp.h"
#include "../inc/ui.h"
#include "../inc/audio.h"
#include "../inc/sensor.h"
#include "../inc/lighting.h"
#include "../inc/temperature.h"
#include "../inc/accelerator.h"
#include "../inc/led_controller.h"
#include "../inc/blinking_led.h"
#include "lpc17xx_timer.h"

int main(void) {
    bsp_init();

    led_controller_init();
    sensor_init();
    temperature_init();
    lighting_init();
    accelerator_init();
    ui_init();
    audio_init();
    blinking_led_init();

    uint32_t last_temp_read_time = 0U;
    uint8_t temperature_level[16] = {'-', '-', '\0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t lux_level[16]         = {'-', '-', '\0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t acc_state[16]         = {'-', '-', '\0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    uint8_t old_temp_buf[16] = {'-', '-', '\0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t old_lux_buf[16]  = {'-', '-', '\0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t old_acc_buf[16]  = {'-', '-', '\0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    uint8_t old_moisture = 0U;

    for (;;) {
        uint8_t moisture_level = sensor_get_level();

        if ((moisture_level != old_moisture) && (getIsMenuOpened() == FALSE)) {
            setRedrawFlag(TRUE);
            old_moisture = moisture_level;
        }

        if ((getTicks() - last_temp_read_time) >= 1000U) {
            custom_strcpy(old_temp_buf, temperature_level, 16U);
            custom_strcpy(old_lux_buf, lux_level, 16U);
            custom_strcpy(old_acc_buf, acc_state, 16U);

            custom_strcpy(temperature_level, read_temperature(), 16U);
            custom_strcpy(lux_level, read_light(), 16U);
            custom_strcpy(acc_state, read_accelerator(), 16U);
            
            last_temp_read_time = getTicks();
            
            if (((custom_strcmp(temperature_level, old_temp_buf) != 0) || 
                 (custom_strcmp(lux_level, old_lux_buf) != 0) || 
                 (custom_strcmp(acc_state, old_acc_buf) != 0)) && 
                (getIsMenuOpened() == FALSE)) {
                
                setRedrawFlag(TRUE);
            }
        }

        ui_process_input();
        
        Bool bIsAlarm = ui_alarm(temperature_level, lux_level, acc_state, moisture_level);
        
        blinking_led_update(bIsAlarm, getTicks());
        if (bIsAlarm == TRUE) {
            audio_play_warning();
        }
        
        led_controller_update_moisture(moisture_level);
        
        ui_display(temperature_level, lux_level, acc_state, moisture_level);
    }
}
