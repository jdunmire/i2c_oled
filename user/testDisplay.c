/*
 * testDisplay.c - minimal test of the i2c_oled functions
 *
 * Uses all of the i2c_oled functions.
 *
 */
#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <mem.h>
#include "driver/i2c_oled.h"
#include "user_config.h"
#include "bitmap.h"

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    char buffer[128];
    int line = 0;
    i2c_init();
    OLED_Init();
    
    OLED_Print(0, 0, "Test 1 - ", 1);
    for (line = 0; line < 8; line++)
    {
        os_sprintf(buffer, "Line %d", line);
        OLED_Print(11, line, buffer, 1);
    }
    os_delay_us(1000000);
    OLED_CLS();

    OLED_Print(0, 1, "Test 2 - ", 1);
    for (line = 0; line < 4; line++)
    {
        os_sprintf(buffer, "Line %d", line);
        OLED_Print(8, line, buffer, 2);
    }
    os_delay_us(1000000);
    OLED_CLS();

    OLED_Print(0, 0, "Test 3 - wrap", 1);
    OLED_Print(0, 2, "123456789012345678901234567890", 1);
    OLED_Print(0, 2, "12345678901234567890", 2);

    os_delay_us(1000000);

    OLED_CLS();

    // Test #4, bit map
    OLED_DrawBMP(0, 0, BITMAP_COLUMNS , BITMAP_ROWS, bitmap);

    os_delay_us(100000);

    // repeat by restarting
    system_restart();

}
