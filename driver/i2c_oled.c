/*
* Oryginal by:
* Author : Aaron Lee
* Version: 1.00
* Date   : 2014.3.24
* Email  : hello14blog@gmail.com
* Modification: none
* Mod by reaper7
* found at http://bbs.espressif.com/viewtopic.php?f=15&t=31
*/
#include "driver/i2c.h"
#include "driver/i2c_oled.h"
#include "driver/i2c_oled_fonts.h"
#include "osapi.h"
#include "user_config.h"  // OLED_ROTATE_180 can be defined in this file

#ifdef OLED_ROTATE_180
static unsigned char
reverse(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}
#define row(i, max) (max - i)
#else
#define reverse(b) (b)
#define row(i, max) (i)
#endif

bool ICACHE_FLASH_ATTR
OLED_writeReg(uint8_t reg_addr,uint8_t val)
{
  i2c_start();
  i2c_writeByte(OLED_ADDRESS);
  if(!i2c_check_ack()) {
    i2c_stop();
    return 0;
  }
  i2c_writeByte(reg_addr);
  if(!i2c_check_ack()) {
    i2c_stop();
    return 0;
  }
  i2c_writeByte(val&0xff);
  if(!i2c_check_ack()) {
    i2c_stop();
    return 0;
  }
  i2c_stop();

  return 1;
}

void ICACHE_FLASH_ATTR
OLED_writeCmd(unsigned char I2C_Command)
{
  OLED_writeReg(0x00,I2C_Command);
}

void ICACHE_FLASH_ATTR
OLED_writeDat(unsigned char I2C_Data)
{
    OLED_writeReg(0x40,I2C_Data);
}

void ICACHE_FLASH_ATTR
OLED_Init(void)
{
    os_delay_us(60000);
    os_delay_us(40000);

  OLED_writeCmd(0xae); // turn off oled panel
  OLED_writeCmd(0x00); // set low column address 
  OLED_writeCmd(0x10); // set high column address 
  OLED_writeCmd(0x40); // set start line address 
  OLED_writeCmd(0x81); // set contrast control register 

  OLED_writeCmd(0xa0);
  OLED_writeCmd(0xc0);

  OLED_writeCmd(0xa6); // set normal display 
  OLED_writeCmd(0xa8); // set multiplex ratio(1 to 64) 
  OLED_writeCmd(0x3f); // 1/64 duty 
  OLED_writeCmd(0xd3); // set display offset 
  OLED_writeCmd(0x00); // not offset 
  OLED_writeCmd(0xd5); // set display clock divide ratio/oscillator frequency 
  OLED_writeCmd(0x80); // set divide ratio 
  OLED_writeCmd(0xd9); // set pre-charge period 
  OLED_writeCmd(0xf1); 
  OLED_writeCmd(0xda); // set com pins hardware configuration 
  OLED_writeCmd(0x12); 
  OLED_writeCmd(0xdb); // set vcomh 
  OLED_writeCmd(0x40); 
  OLED_writeCmd(0x8d); // set Charge Pump enable/disable 
  OLED_writeCmd(0x14); // set(0x10) disable
  OLED_writeCmd(0xaf); // turn on oled panel

  OLED_Fill(0x00);  //OLED CLS

  return;
}

void ICACHE_FLASH_ATTR
OLED_SetPos(unsigned char x1, unsigned char y1)
{
#ifdef OLED_ROTATE_180
    unsigned char y = 8 - y1;
    unsigned char x = 127 - x1;
#else
    unsigned char y = y1;
    unsigned char x = x1;
#endif

    OLED_writeCmd(0xb0+y);
    OLED_writeCmd(((x&0xf0)>>4)|0x10);
    OLED_writeCmd((x&0x0f)); //|0x01);
}

void ICACHE_FLASH_ATTR
OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        OLED_writeCmd(0xb0+m);      //page0-page1
        OLED_writeCmd(0x00);        //low column start address
        OLED_writeCmd(0x10);        //high column start address
        for(n=0;n<132;n++)
        {
            OLED_writeDat(fill_Data);
        }
    }
}

void ICACHE_FLASH_ATTR
OLED_CLS(void)
{
    OLED_Fill(0x00);
}

void ICACHE_FLASH_ATTR
OLED_ON(void)
{
    OLED_writeCmd(0X8D);
    OLED_writeCmd(0X14);
    OLED_writeCmd(0XAF);
}

void ICACHE_FLASH_ATTR
OLED_OFF(void)
{
    OLED_writeCmd(0X8D);
    OLED_writeCmd(0X10);
    OLED_writeCmd(0XAE);
}

/**
 * OLED_ShowStr
 * Display string starting at x,y. x is in bits, y is in bytes.
 * Wrap to next line as needed.
 *
 */
void ICACHE_FLASH_ATTR
OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char fontID)
{
    unsigned char c = 0;
    unsigned char index = 0;
    unsigned char cStartCol = 0;
    unsigned char charCol = 0;

    switch(fontID)
    {
        case 1:  // Small font, 6x8
        {
#ifdef OLED_ROTATE_180
            cStartCol = 6;
#endif
            while (ch[index] != '\0')
            {
                c = ch[index] - 32;
                if (x > 125)
                {
                    x = 0;
                    y++;
                    if (y > 7)
                    {
                        break;
                    }
                }
                OLED_SetPos(x + cStartCol, y);
                for (charCol = 0; charCol < 6; charCol++)
                {
                    OLED_writeDat(reverse(F6x8[c][row(charCol, 5)]));
                }
                x += 6;
                index++;
            }
        }
        break;

        case 2:  // Large font, 8x16
        {
#ifdef OLED_ROTATE_180
            cStartCol = 8;
#endif
            while (ch[index] != '\0')
            {
                c = ch[index] - 32; // Array has only printable characters
                if(x > 127)  // wrap at end of line
                {
                    x = 0;
                    y += 2;
                    if (y > 7)
                    {
                        break;
                    }
                }
                OLED_SetPos(x + cStartCol, y);
                for (charCol = 0; charCol < 8; charCol++)
                {
                    OLED_writeDat(reverse(F8X16[c * 16 + row(charCol, 7)]));
                }
                OLED_SetPos(x + cStartCol, y + 1);
                for (charCol = 0; charCol < 8; charCol++)
                {
                    OLED_writeDat(reverse(F8X16[c * 16 + row(charCol, 7) + 8]));
                }
                x += 8;
                index++;
            }
        }
        break;
    }
}

/**
 * OLED_Print
 * Display string starting at row y, column x.
 * x and y are in both in characters.
 * This is largely a convience wrapper for OLED_ShowStr() so that x
 * and y can be in the same units.
 */
void ICACHE_FLASH_ATTR
OLED_Print(unsigned char x, unsigned char y, unsigned char ch[], unsigned char fontID)
{
  uint8_t step;
  uint8_t line = y;
  if (fontID==1)
  {
    step = x*6; 
  }
  else if (fontID==2)
  {
    step = x*8; 
    line = 2*y;
  }
  else 
  {
    step = x;
  }
  OLED_ShowStr(step,line,ch,fontID);
}

/**
 * OLED_DrawBMP - Display a bitmap image.
 */
void ICACHE_FLASH_ATTR
OLED_DrawBMP(
        unsigned char x0,
        unsigned char y0,
        unsigned char numX,
        unsigned char numY,
        unsigned char bitmap[]
        )
{
    unsigned int j = 0;
    unsigned char x ,y;

#ifdef OLED_ROTATE_180
    unsigned int max = ((numX) * (numY)) - 1;

    for (y = (y0 + 1); y <= (y0 + numY); y++)
    {
        OLED_SetPos((numX + x0), y);
        j = 0;
        for (x = (x0 + numX); x > x0; x--)
        {
            OLED_writeDat(reverse(bitmap[(numX * y) - j++]));
        }
    }

#else
    for (y = y0; y < (y0 + numY); y++)
    {
        OLED_SetPos(x0, y);
        for (x = x0; x < (x0 + numX); x++)
        {
            OLED_writeDat(reverse(bitmap[j++]));
        }
    }
#endif
}

