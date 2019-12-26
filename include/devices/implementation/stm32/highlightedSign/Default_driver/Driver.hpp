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
                }

        unsigned char setBlinkingLevel(BlinkingLevel level, unsigned short distanceInCm = 0) override {
            if (lastBlinkingLevel != level) {
                lastBlinkingLevel = level;
                blinker.setNewBlinkingLevel(level);
            }

            const bool highlightState = blinker.updateAndGetActualHighlightState();
            digitalWrite(highlightState, config.highlightSteeringPin);
            return 0;
        }
    };
}