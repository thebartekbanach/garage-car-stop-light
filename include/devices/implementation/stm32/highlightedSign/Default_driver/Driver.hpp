#pragma once
#include <Arduino.h>

#include "../../../../abstract/HighlightedSign.hpp"

#include "Configuration.hpp"

#include "utils/Blinker.hpp"

namespace Devices::HighlightedSign::Implementation::Stm32::Default_driver {
    class DefaultDriver_HiglightedSign: public Abstract::IHighlightedSign {
        private:
            const DefaultDriver_Configuration &config;
            BlinkingLevel lastBlinkingLevel = BlinkingLevel::OFF;
            Blinker blinker;

        public:
            DefaultDriver_HiglightedSign(const DefaultDriver_Configuration &_configuration):
                config(_configuration),
                blinker(config.lightingSchemes) {
                    pinMode(
                        config.highlightSteeringPin,
                        config.highlightPinWiringMode
                    );

                    if (config.mirrorBlinkingOnLedBuiltin) {
                        pinMode(
                            LED_BUILTIN,
                            WiringPinMode::OUTPUT
                        );
                    }
                }

        const HighlightSetResult setBlinkingLevel(const BlinkingLevel &level, const unsigned short &distanceInCm) override {
            if (lastBlinkingLevel != level) {
                lastBlinkingLevel = level;
                blinker.setNewBlinkingLevel(level);
            }

            const bool highlightState = blinker.updateAndGetActualHighlightState();
            const bool lightState = highlightState == true
                ? config.turnedOnElectricState
                : !config.turnedOnElectricState;

            digitalWrite(
                config.highlightSteeringPin,
                lightState
            );

            if (config.mirrorBlinkingOnLedBuiltin) {
                digitalWrite(
                    LED_BUILTIN,
                    lightState
                );
            }
                
            return {
                .level = level,
                .actualState = !lightState,
                .error = 0
            };
        }
    };
}