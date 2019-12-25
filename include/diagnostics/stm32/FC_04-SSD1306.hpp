#pragma once

#define FC_04_SSD1306D_DIAGNOSTICS_ENABLED false

#if FC_04_SSD1306D_DIAGNOSTICS_ENABLED == true
#define DIAGNOSTICS_MODE_ENABLED

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

bool lastState = false;
bool meansurements[1500] { 0 };
unsigned short counter = 0;

void drawEngineState(const bool state) {
    if (lastState == state) return;
    else lastState = state;
    
    display.clearBuffer();
    
    if (state == true) {
        display.drawStr(0, 20, "Sound detected!");
    } else {
        display.drawStr(0, 20, "No sound...");
    }

    display.sendBuffer();
}

void setup(void) {
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 20, "Try to say something...");
    display.sendBuffer();
    delay(500);

    pinMode(PB4, INPUT_PULLUP);
}

void loop(void) {
    if (counter >= 1500) {
        counter = 0;
    }
    
    meansurements[counter] = !digitalRead(PB4);
    counter++;

    for (unsigned short i = 0; i < 1500; ++i) {
        if (meansurements[i] == true) {
            drawEngineState(true);
            return;
        }
    }

    drawEngineState(false);
}

#endif
