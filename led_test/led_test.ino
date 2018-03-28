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


// Port 3 is connected to LED on Step FPGA MAX 10 board

uint8_t led_index;

//---------------------------------------------------------------------------------
// setup()
//---------------------------------------------------------------------------------

void setup()  {
  led_index = 1;
  P3_DIRECTION = 0xFF; // Set Port 3 direction
}

//---------------------------------------------------------------------------------
// loop()
//---------------------------------------------------------------------------------

void loop()  {
  P3 = led_index;

  if (led_index == 0x80) {
      led_index = 1;  
  } else {
      led_index = led_index << 1;
  }

  delay(1000);
}

