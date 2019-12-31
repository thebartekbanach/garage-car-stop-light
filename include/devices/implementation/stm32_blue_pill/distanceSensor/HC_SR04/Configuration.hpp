#pragma once

namespace Devices::DistanceSensor::Implementation::Stm32::HC_SR04 {
    struct HC_SR04_Configuration {
        unsigned short triggerPin;
        unsigned short echoPin;
        unsigned short numberOfPingsPerScan = 10;
        unsigned short maximumAllowedNumberOfFaultPingsPerScan = 5;
        unsigned short maximumAllowedDistance = 200;
    };
}