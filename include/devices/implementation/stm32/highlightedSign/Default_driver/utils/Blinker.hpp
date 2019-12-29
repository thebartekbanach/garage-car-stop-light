#pragma once
#include "../../../../../abstract/HighlightedSign.hpp"

#include "../../../utils/Timer.hpp"

#include "../Configuration.hpp"

namespace Devices::HighlightedSign::Implementation::Stm32::Default_driver {
    class Blinker {
        private:
            const LightingSchemes &lightingSchemes;

            BlinkingLevel blinkingLevel = BlinkingLevel::STOP;
            bool actualLightState = false;
            Timer lightingTimer;

            const LightingScheme& getLightingSchemeForLevel(const BlinkingLevel &level) {
                switch (level) {
                    case BlinkingLevel::MEDIUM:
                        return lightingSchemes.mediumLightingScheme;

                    case BlinkingLevel::FAR:
                        return lightingSchemes.farLightingScheme;

                    default: /* BlinkingLevel::NEAR */
                        return lightingSchemes.nearLightingScheme;
                }
            }

            void setupTimerUsingScheme(const LightingScheme& scheme) {
                lightingTimer.start(
                    actualLightState == true
                        ? scheme.onTime
                        : scheme.offTime
                );
            }

        public:
            Blinker(const LightingSchemes &_lightingSchemes):
                lightingSchemes(_lightingSchemes) {}

            void setNewBlinkingLevel(const BlinkingLevel &newLevel) {
                blinkingLevel = newLevel;

                if (blinkingLevel == BlinkingLevel::OFF) {
                    actualLightState = false;
                    return;
                }

                actualLightState = true;
                setupTimerUsingScheme(getLightingSchemeForLevel(blinkingLevel));
            }

            bool updateAndGetActualHighlightState() {
                if (blinkingLevel == BlinkingLevel::OFF) {
                    return false;
                }

                if (blinkingLevel == BlinkingLevel::STOP) {
                    return true;
                }

                if (lightingTimer.done()) {
                    actualLightState = !actualLightState;
                    setupTimerUsingScheme(getLightingSchemeForLevel(blinkingLevel));
                }

                return actualLightState;
            }
    };
}