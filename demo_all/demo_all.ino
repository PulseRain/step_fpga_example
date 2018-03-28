//*********************************************************************************
// Copyright (c) 2018 PulseRain Technology, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//*********************************************************************************

#include "M10PWM.h"
#include "M10SevenSeg.h"
#include "M10PS2.h"
#include "M10LCD.h"


// init sequence for LCD
uint16_t lcd_init_sequence [] = {
    0x8011,
  
    0x00B1,
    0x0105,
    0x013C,
    0x013C,
     
    0x00B2,
    0x0105,
    0x013C,
    0x013C,
  
    0x00B3,
    0x0105,
    0x013C,
    0x013C,
    0x0105,
    0x013C,
    0x013C,
    
    0x00B4,
    0x0103,
      
    0x00C0,
    0x0128,
    0x0108,
    0x0104,
  
    0x00C1,
    0x01C0,
  
    0x00C2,
    0x010D,
    0x0100,
  
    0x00C3,
    0x018D,
    0x012A,
  
    0x00C4,
    0x018D,
    0x01EE,
  
    0x00C5,
    0x011A,
    
    0x0036,
    0x01C0,
    
    0x00E0,
    0x0104,
    0x0122,
    0x0107,
    0x010A,
    0x012E,
    0x0130,
    0x0125,
    0x012A,
    0x0128,
    0x0126,
    0x012E,
    0x013A,
    0x0100,
    0x0101,
    0x0103,
    0x0113,
    
    0x00E1,
    0x0104,
    0x0116,
    0x0106,
    0x010D,
    0x012D,
    0x0126,
    0x0123,
    0x0127,
    0x0127,
    0x0125,
    0x012D,
    0x013B,
    0x0100,
    0x0101,
    0x0104,
    0x0113,
  
    0x003A,
    0x0106, // 18 bit True Color
    0x0029

};


//---------------------------------------------------------------------------------
// lcd_init()
//---------------------------------------------------------------------------------

void lcd_init() 
{
    uint16_t i, t, total = sizeof(lcd_init_sequence) >> 1;

    LCD.reset();
        
    for (i = 0; i < total; ++i) {
        t = lcd_init_sequence[i];
        LCD_CSR = (t >> 8) & 0x01;
        nop_delay (10);
        if (t & 0x8000) {
            delay(50);
        }
        
        LCD_DATA = t & 0xFF;
        nop_delay (10);
        while(LCD_CSR == 0);
        
    } // End of for loop    
    
}


//---------------------------------------------------------------------------------
// lcd_set_active_region()
//
// Remarks:
//    setup active region
//---------------------------------------------------------------------------------

void lcd_set_active_region(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{    
    LCD.writeCMD(0x2A);
    LCD.writeDAT(0x0);
    LCD.writeDAT(x_start);
    LCD.writeDAT(0x0);
    LCD.writeDAT(x_end);

    LCD.writeCMD(0x2B);
    LCD.writeDAT(0x0);
    LCD.writeDAT(y_start);
    LCD.writeDAT(0x0);
    LCD.writeDAT(y_end);

    LCD.writeCMD(0x2C);
}

//---------------------------------------------------------------------------------
// flash_byte_read()
//
// Remarks:
//    read data from UFM
//---------------------------------------------------------------------------------

uint8_t flash_byte_read (uint32_t addr)
{
  uint8_t t;

  t = (addr >> 24) & 0xff;
  FLASH_READ_DATA = t;
  
  t = (addr >> 16) & 0xff;
  FLASH_READ_DATA = t;
  
  t = (addr >> 8) & 0xff;
  FLASH_READ_DATA = t;
  
  t = addr & 0xff;
  FLASH_READ_DATA = t;

  nop_delay (10);
  
  FLASH_READ_CSR = 0;
  nop_delay (10);
    
  while(FLASH_READ_CSR == 0);
  t = FLASH_READ_DATA;

  return t;
  
}

//---------------------------------------------------------------------------------
// show_logo()
//
// Remarks:
//    display logo
//---------------------------------------------------------------------------------

void show_logo()
{
    uint8_t i,j,k,t;
    const uint8_t x = 128, y = 154;
    const uint8_t x_total = 132, y_total = 162;
    uint32_t addr = 0x8000;

     
    lcd_set_active_region(0,0,x_total-1,y_total-1);
    
    for (i = 0; i < x_total; ++i) {
      for (j = 0; j < y_total; ++j) {
        for (k = 0; k < 3; ++k) {
           LCD.writeDAT(0xff);
        }
      }
    }
    
    lcd_set_active_region((x_total - x) >> 1, (y_total- y) >> 1,x-1 +((x_total - x) >> 1),y-1 + ((y_total- y) >> 1));
    
    for (i=0;i<x;i++) {
        for (j=0;j<y;j++) {
           // lcd_write_16bit(0x00ff);
           // lcd_write_dat (0);

            for (k = 0; k < 3; ++k) {
              t = flash_byte_read (addr++);
              LCD.writeDAT(t);
             // Serial.print (addr, HEX);
             // Serial.write ("   ");
             // Serial.println(t, HEX);
            }
        }
    }
} // End of show_logo()




uint8_t old_count = 0;
uint8_t count = 0;


//---------------------------------------------------------------------------------
// setup()
//---------------------------------------------------------------------------------

void setup() {

  uint8_t t, i;
  Serial.begin (115200);

  SEVEN_SEG.init();
  PS2.init();
  PWM.resolution (0, 938);
  
  ROTARY_ENCODER = 0x0;
  P3_DIRECTION = 0xFF;

  lcd_init();
  show_logo();
  
  PWM.dutyCycle (0, 255, 0);

  Serial.write ("Hello world!");
}


//---------------------------------------------------------------------------------
// loop()
//---------------------------------------------------------------------------------

void loop() {

  uint8_t t;

  old_count = count;
  count = ROTARY_ENCODER;
  SEVEN_SEG.byteHex (count);

  if (old_count != count) {
     PWM.dutyCycle (0, count, 255-count);
  }

  P3 = (1 << (count >> 5)) - 1;


  if (PS2.dataAvailable()) {
    t = PS2.read();
  
    if (t == 0xF0) {
      Serial.print(t, HEX);
    } else {
      Serial.println(t, HEX);
    }
  }

}

