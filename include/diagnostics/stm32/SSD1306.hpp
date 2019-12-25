#pragma once

#define SSD1306_DIAGNOSTICS_ENABLED false

#if SSD1306_DIAGNOSTICS_ENABLED == true
#define DIAGNOSTICS_MODE_ENABLED

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
    u8g2.begin();
}

void loop(void) {
    u8g2.clearBuffer();                   // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);   // choose a suitable font
    u8g2.drawStr(0, 20, "Hello world!");  // write something to the internal memory
    u8g2.sendBuffer();                    // transfer internal memory to the display
    delay(1000);
}

#endif
