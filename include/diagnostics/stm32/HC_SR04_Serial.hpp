#pragma once

#define HC_SR04_DIAGNOSTICS_ENABLED false

#if HC_SR04_DIAGNOSTICS_ENABLED == true
#define DIAGNOSTICS_MODE_ENABLED

#include <Arduino.h>

#include <HCSR04.h>

UltraSonicDistanceSensor* sensor = nullptr;

void setup() {
    sensor = new UltraSonicDistanceSensor(PB10, PB11);
}

void loop() {
    delay(500);
    
    double distance = sensor->measureDistanceCm();

    Serial.print(distance);
}

#endif