#include "../inc/ui.h"
#include "oled.h"
#include "joystick.h"
#include "eeprom.h"
#include "lpc17xx_timer.h"
#include <stddef.h>

static uint8_t name_idx = 0U;

static const uint8_t* chosenName;
static const uint8_t names[5][10] = {
    {(uint8_t)'G', (uint8_t)'a', (uint8_t)'b', (uint8_t)'r', (uint8_t)'y', (uint8_t)'s', (uint8_t)'i', (uint8_t)'a', (uint8_t)'\0', 0U},
    {(uint8_t)'R', (uint8_t)'a', (uint8_t)'d', (uint8_t)'e', (uint8_t)'k', (uint8_t)'\0', 0U, 0U, 0U, 0U},
    {(uint8_t)'J', (uint8_t)'a', (uint8_t)'c', (uint8_t)'e', (uint8_t)'k', (uint8_t)'\0', 0U, 0U, 0U, 0U},
    {(uint8_t)'K', (uint8_t)'w', (uint8_t)'i', (uint8_t)'a', (uint8_t)'t', (uint8_t)'e', (uint8_t)'k', (uint8_t)'\0', 0U, 0U},
    {(uint8_t)'A', (uint8_t)'n', (uint8_t)'d', (uint8_t)'r', (uint8_t)'z', (uint8_t)'e', (uint8_t)'j', (uint8_t)'\0', 0U, 0U}
};

static Bool bNeedsRedraw = TRUE;

typedef enum {
    MAIN_MENU = 0,
    NAME_MENU,
    STATS_MENU
} MenuState;

typedef enum {
    MIN_TEMP = 0,
    MAX_TEMP,
    MIN_LUX,
    MAX_LUX,
    MIN_MOISTURE,
    MAX_MOISTURE,
    PARAM_COUNT
} EditParams;

static MenuState current_state = MAIN_MENU;
static EditParams current_param = MIN_TEMP;

static PlantConditions expected_conditions = {
    .min_temp = 20, .max_temp = 30, 
    .min_lux = 0, .max_lux = 800,
    .min_moisture = 8, .max_moisture = 15
};

static int custom_atoi(const uint8_t* str) {
    int res = 0;
    int sign = 1;
    
    if (str != NULL) {
		uint32_t i = 0U;
        if (str[0] == (uint8_t)'-') {
            sign = -1;
            i++;
        }
        while ((str[i] >= (uint8_t)'0') && (str[i] <= (uint8_t)'9')) {
            res = (res * 10) + ((int)str[i] - (int)'0');
            i++;
        }
    }
    return res * sign;
}

static uint32_t custom_strlen(const uint8_t* str) {
    uint32_t len = 0U;
    if (str != NULL) {
        while (str[len] != (uint8_t)'\0') {
            len++;
        }
    }
    return len;
}

int custom_strcmp(const uint8_t* str1, const uint8_t* str2) {
    int result = 0;
    
    if ((str1 != NULL) && (str2 != NULL)) {
		uint32_t i = 0U;
        while ((str1[i] != (uint8_t)'\0') && (str1[i] == str2[i])) {
            i++;
        }
        if (str1[i] > str2[i]) {
            result = 1;
        } else if (str1[i] < str2[i]) {
            result = -1;
        } else {
            result = 0;
        }
    }
    return result;
}

void custom_strcpy(uint8_t* dest, const uint8_t* src, uint32_t max_len) {
    if ((dest != NULL) && (src != NULL) && (max_len > 0U)) {
		uint32_t i = 0U;
        while ((i < (max_len - 1U)) && (src[i] != (uint8_t)'\0')) {
            dest[i] = src[i];
            i++;
        }
        dest[i] = (uint8_t)'\0';
    }
}

void ui_init(void) {
	static uint8_t memoryName[11] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
    oled_init();
    oled_clearScreen(OLED_COLOR_BLACK);
    joystick_init();
    eeprom_init();

    (void)eeprom_read(memoryName, 0, 10);
    memoryName[10] = (uint8_t)'\0';

    if ((memoryName[0] != 0xFFU) && (memoryName[0] != 0x00U)) {
        chosenName = memoryName;
    } else {
        chosenName = (const uint8_t*)"Brak";
    }

    bNeedsRedraw = TRUE;
}

static void main_menu(uint8_t* temperature, uint8_t* lux, uint8_t* acc, uint8_t moisture) {
    uint8_t moist_str[6];
    intToString((int)moisture, moist_str, 6U, 10U);

    oled_putString(1, 1, (const uint8_t*)"Witaj:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_putString(40, 1, chosenName, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

    oled_putString(1, 14, (const uint8_t*)"Temp:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_putString(40, 14, temperature, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

    oled_putString(1, 27, (const uint8_t*)"Lux:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_putString(40, 27, lux, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

    oled_putString(1, 40, (const uint8_t*)"Wilg:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_putString(40, 40, moist_str, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

    oled_putString(1, 53, (const uint8_t*)"Acc:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    oled_putString(40, 53, acc, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

static void name_menu(void) {
    int i = 0;
    oled_putString(1, 1, (const uint8_t*)"Wybierz imie", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    
    for (i = 0; i < 5; i++) {
        uint32_t text_color = ((uint8_t)i == name_idx) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
        uint32_t bg_color = ((uint8_t)i == name_idx) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
        oled_putString(1, 10 + (i * 8), names[i], text_color, bg_color);
    }
}

static void stats_menu(void) {
    uint8_t min_t[6];
    uint8_t max_t[6];
    uint8_t min_l[6];
    uint8_t max_l[6];
    uint8_t min_m[6];
    uint8_t max_m[6];

    intToString(expected_conditions.min_temp, min_t, 6U, 10U);
    intToString(expected_conditions.max_temp, max_t, 6U, 10U);
    intToString(expected_conditions.min_lux, min_l, 6U, 10U);
    intToString(expected_conditions.max_lux, max_l, 6U, 10U);
    intToString(expected_conditions.min_moisture, min_m, 6U, 10U);
    intToString(expected_conditions.max_moisture, max_m, 6U, 10U);

    oled_putString(1, 1, (const uint8_t*)"- USTAWIENIA -", OLED_COLOR_WHITE, OLED_COLOR_BLACK);

    oled_putString(1, 16, (const uint8_t*)"Tmp:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    uint32_t tc = (current_param == MIN_TEMP) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
    uint32_t bg = (current_param == MIN_TEMP) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
    oled_putString(32, 16, min_t, tc, bg);
    oled_putString(46, 16, (const uint8_t*)"-", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    tc = (current_param == MAX_TEMP) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
    bg = (current_param == MAX_TEMP) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
    oled_putString(56, 16, max_t, tc, bg);

    oled_putString(1, 32, (const uint8_t*)"Lux:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    tc = (current_param == MIN_LUX) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
    bg = (current_param == MIN_LUX) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
    oled_putString(32, 32, min_l, tc, bg);
    oled_putString(50, 32, (const uint8_t*)"-", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    tc = (current_param == MAX_LUX) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
    bg = (current_param == MAX_LUX) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
    oled_putString(60, 32, max_l, tc, bg);

    oled_putString(1, 48, (const uint8_t*)"Wilg:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    tc = (current_param == MIN_MOISTURE) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
    bg = (current_param == MIN_MOISTURE) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
    oled_putString(32, 48, min_m, tc, bg);
    oled_putString(46, 48, (const uint8_t*)"-", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    tc = (current_param == MAX_MOISTURE) ? OLED_COLOR_BLACK : OLED_COLOR_WHITE;
    bg = (current_param == MAX_MOISTURE) ? OLED_COLOR_WHITE : OLED_COLOR_BLACK;
    oled_putString(56, 48, max_m, tc, bg);
}

void ui_process_input(void) {
    static uint8_t prev_joy = 0U;
	static uint8_t joy = 0U;
    joy = joystick_read();

    if (((joy & JOYSTICK_RIGHT) != 0U) && ((prev_joy & JOYSTICK_RIGHT) == 0U)) {
        current_state++;
        if (current_state > STATS_MENU) {
            current_state = MAIN_MENU;
        }
        current_param = MIN_TEMP;
        bNeedsRedraw = TRUE;
    }

    switch (current_state) {
        case MAIN_MENU:
            break;
        case NAME_MENU:
            if (((joy & JOYSTICK_CENTER) != 0U) && ((prev_joy & JOYSTICK_CENTER) == 0U)) {
                name_idx++;
                if (name_idx >= 5U) { name_idx = 0U; }
                chosenName = names[name_idx];
                (void)eeprom_write((uint8_t *)(uintptr_t)chosenName, 0, (uint16_t)(custom_strlen(chosenName) + 1U));
                bNeedsRedraw = TRUE;
            }
            break;
        case STATS_MENU:
            if (((joy & JOYSTICK_CENTER) != 0U) && ((prev_joy & JOYSTICK_CENTER) == 0U)) {
                current_param++;
                if (current_param >= PARAM_COUNT) { current_param = MIN_TEMP; }
                bNeedsRedraw = TRUE;
            }

            if (((joy & JOYSTICK_UP) != 0U) && ((prev_joy & JOYSTICK_UP) == 0U)) {
                if ((current_param == MIN_TEMP) && (expected_conditions.min_temp < (expected_conditions.max_temp - 1U))) {
                    expected_conditions.min_temp++;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MAX_TEMP) && (expected_conditions.max_temp < 50U)) {
                    expected_conditions.max_temp++;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MIN_LUX) && (expected_conditions.min_lux < (expected_conditions.max_lux - 5U))) {
                    expected_conditions.min_lux += 5U;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MAX_LUX) && (expected_conditions.max_lux < 2000U)) {
                    expected_conditions.max_lux += 5U;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MIN_MOISTURE) && (expected_conditions.min_moisture < (expected_conditions.max_moisture - 1U))) {
                    expected_conditions.min_moisture++;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MAX_MOISTURE) && (expected_conditions.max_moisture < 16U)) {
                    expected_conditions.max_moisture++;
                    bNeedsRedraw = TRUE;
                }
                else { }
            }

            if (((joy & JOYSTICK_DOWN) != 0U) && ((prev_joy & JOYSTICK_DOWN) == 0U)) {
                if ((current_param == MIN_TEMP) && (expected_conditions.min_temp > 0U)) {
                    expected_conditions.min_temp--;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MAX_TEMP) && (expected_conditions.max_temp > (expected_conditions.min_temp + 1U))) {
                    expected_conditions.max_temp--;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MIN_LUX) && (expected_conditions.min_lux > 0U)) {
                    expected_conditions.min_lux -= 5U;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MAX_LUX) && (expected_conditions.max_lux > (expected_conditions.min_lux + 5U))) {
                    expected_conditions.max_lux -= 5U;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MIN_MOISTURE) && (expected_conditions.min_moisture > 1U)) {
                    expected_conditions.min_moisture--;
                    bNeedsRedraw = TRUE;
                }
                else if ((current_param == MAX_MOISTURE) && (expected_conditions.max_moisture > (expected_conditions.min_moisture + 1U))) {
                    expected_conditions.max_moisture--;
                    bNeedsRedraw = TRUE;
                }
                else { }
            }
            break;
        default:
            break;
    }
    prev_joy = joy;
}

void ui_display(uint8_t* temperature, uint8_t* lux, uint8_t* acc, uint8_t moisture) {
    if (bNeedsRedraw == TRUE) {
        oled_clearScreen(OLED_COLOR_BLACK);
        switch (current_state) {
            case MAIN_MENU:
                main_menu(temperature, lux, acc, moisture);
                break;
            case NAME_MENU:
                name_menu();
                break;
            case STATS_MENU:
                stats_menu();
                break;
            default:
                break;
        }
    }
    bNeedsRedraw = FALSE;
}

Bool ui_alarm(const uint8_t* temperature, const uint8_t* lux, const uint8_t* acc, uint8_t moisture) {
    Bool bIsAlarm = FALSE;

    if ((temperature[0] != (uint8_t)'-') && (lux[0] != (uint8_t)'-') && (acc[0] != (uint8_t)'-')) {
        int current_temp = custom_atoi(temperature);
        int current_lux = custom_atoi(lux);
        Bool bIsTilted = (custom_strcmp(acc, (const uint8_t*)"stable") != 0) ? TRUE : FALSE;

        if ((current_temp < (int)expected_conditions.min_temp) || (current_temp > (int)expected_conditions.max_temp) ||
			(current_lux < (int)expected_conditions.min_lux) || (current_lux > (int)expected_conditions.max_lux) ||
			(moisture < (uint8_t)expected_conditions.min_moisture) || (moisture > (uint8_t)expected_conditions.max_moisture) ||
			(bIsTilted == TRUE)) {
            
            bIsAlarm = TRUE;
        }
    }
    return bIsAlarm;
}

void intToString(int value, uint8_t* pBuf, uint32_t len, uint32_t base) {
    static const uint8_t pAscii[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int tmpValue = value;
    int tmpValueParam = value; 

    if ((pBuf != NULL) && (len >= 2U) && (base >= 2U) && (base <= 36U)) {
        int pos = 0;
        if (tmpValueParam < 0) {
            tmpValue = -tmpValue;
            tmpValueParam = -tmpValueParam;
            pBuf[pos] = (uint8_t)'-';
            pos++;
        }

        do {
            pos++;
            tmpValue /= (int)base;
        } while(tmpValue > 0);

        if ((uint32_t)pos <= len) {
            pBuf[pos] = (uint8_t)'\0';
            do {
                pos--;
                pBuf[pos] = pAscii[tmpValueParam % (int)base];
                tmpValueParam /= (int)base;
            } while(tmpValueParam > 0);
        }
    }
}

void setRedrawFlag(Bool flag) {
    bNeedsRedraw = flag;
}

Bool getIsMenuOpened(void) {
    return (current_state != MAIN_MENU);
}
