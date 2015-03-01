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

// square, 24x24 pixels. 24 columns (x) by 3 rows (y)
unsigned char square[] =
{
    0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x55,

    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55,

    0xff, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x55,
};


//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    char buffer[128];
    int line = 0;
    i2c_init();
    OLED_Init();
    
    OLED_Print(2, 0, "Test 1", 1);
    for (line = 0; line < 8; line++)
    {
        os_sprintf(buffer, "Line %d", line);
        OLED_Print(11, line, buffer, 1);
    }
    os_delay_us(1000000);
    OLED_CLS();

    OLED_Print(2, 1, "Test 2", 1);
    for (line = 0; line < 4; line++)
    {
        os_sprintf(buffer, "Line %d", line);
        OLED_Print(8, line, buffer, 2);
    }
    os_delay_us(1000000);
    OLED_CLS();

    OLED_Print(2, 0, "Test 3 - wrap", 1);
    OLED_Print(0, 2, "123456789012345678901234567890", 1);
    OLED_Print(0, 2, "12345678901234567890", 2);

    os_delay_us(1000000);
    OLED_CLS();

    OLED_Print(2, 0, "Test 4 - bitmap", 1);

    OLED_DrawBMP(52, 3, 76, 6, square);

    os_delay_us(100000);
    // repeat by restarting
    system_restart();

}
