#pragma once

#define HC_SR04_SSD1306_DIAGNOSTICS_ENABLED false

#if HC_SR04_SSD1306_DIAGNOSTICS_ENABLED == true
#define DIAGNOSTICS_MODE_ENABLED

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <HCSR04.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
UltraSonicDistanceSensor* sensor = nullptr;

void updateDistanceOnScreen(float newDistance) {
    display.clearBuffer();
    display.drawStr(0, 20, (String(newDistance) + "cm").c_str());
    display.sendBuffer();
}

void setup(void) {
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 20, "Starting...");
    display.sendBuffer();
    delay(500);

    sensor = new UltraSonicDistanceSensor(PB10, PB11);
}

void loop(void) {
    updateDistanceOnScreen(sensor->measureDistanceCm());
    delay(100);
}

#endif
