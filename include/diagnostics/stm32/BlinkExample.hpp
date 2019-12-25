#pragma once

#define BLINK_DIAGNOSTICS_ENABLED false

#if BLINK_DIAGNOSTICS_ENABLED == true
#define DIAGNOSTICS_MODE_ENABLED

#include <Arduino.h>

void setup() {
    pinMode(PC13, OUTPUT);
}

void loop() {
    digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second
    digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
    delay(500);              // wait for a second    
}

#endif