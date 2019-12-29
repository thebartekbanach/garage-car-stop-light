#pragma once
#include <Arduino.h>

namespace Devices::HighlightedSign::Implementation::Stm32::Default_driver {
    struct LightingScheme {
        unsigned short onTime; // in ms
        unsigned short offTime; // in ms
    };

    struct LightingSchemes {
        LightingScheme nearLightingScheme;
        LightingScheme mediumLightingScheme;
        LightingScheme farLightingScheme;
    };

    struct DefaultDriver_Configuration {
        WiringPinMode highlightPinWiringMode;
        unsigned short highlightSteeringPin;
        bool turnedOnElectricState;
        LightingSchemes lightingSchemes;
        bool mirrorBlinkingOnLedBuiltin;
    };
}