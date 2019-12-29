#pragma once
#include <U8g2lib.h>
#include <Wire.h>

#include "../../../../abstract/StateIndicator.hpp"

namespace Devices::StateIndicator::Implementation::Stm32::SSD1306 {
    using Devices::DistanceSensor::Distance;
    using Devices::CarEngineStateSensor::EngineState;
    using Devices::DistanceCalibrator::DistanceCalibrationValue;
    using Devices::HighlightedSign::HighlightSetResult;
    using Devices::HighlightedSign::BlinkingLevel;

    using Core::Configuration;

    class SSD1306_StateIndicator: public Abstract::IStateIndicator {
        private:
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display;
            
            Configuration lastConfiguration;
            Distance lastDistance;
            EngineState lastEngineState;
            DistanceCalibrationValue lastDistanceCalibrationValue;
            HighlightSetResult lastHighlighSetResult;

            bool displayUpdateNeeded(
                const Configuration &configuration,
                const Distance &distance,
                const EngineState &engineState,
                const DistanceCalibrationValue &calibrationValue,
                const HighlightSetResult &highlighSetResult
            ) {
                return
                    configuration.stopDistance != lastConfiguration.stopDistance ||
                    configuration.nearDistance != lastConfiguration.nearDistance ||
                    configuration.mediumDistance != lastConfiguration.mediumDistance ||
                    configuration.farDistance != lastConfiguration.farDistance ||

                    distance.distance != lastDistance.distance ||
                    distance.tooFar != lastDistance.tooFar ||
                    distance.error != lastDistance.error ||

                    engineState.isEngineStarted != lastEngineState.isEngineStarted ||
                    engineState.error != lastEngineState.error ||

                    calibrationValue.value != lastDistanceCalibrationValue.value ||
                    calibrationValue.error != lastDistanceCalibrationValue.error ||

                    highlighSetResult.level != lastHighlighSetResult.level ||
                    highlighSetResult.actualState != lastHighlighSetResult.level ||
                    highlighSetResult.error != lastHighlighSetResult.error;
            }

            void redrawDistance() {
                if (lastDistance.error != 0) {
                    display.drawStr(70, 10, ("ERR" + String((unsigned short)lastDistance.error)).c_str());
                } else if (lastDistance.tooFar) {
                    display.drawStr(70, 10, "too far");
                } else {
                    display.drawStr(80, 10, (String((unsigned short)lastDistance.distance) + "cm").c_str());
                }
            }

            void redrawEngineState() {
                if (lastEngineState.error != 0) {
                    display.drawStr(0, 20, ("Engine: ERR" + String((unsigned short)lastEngineState.error)).c_str());
                } else {
                    display.drawStr(0, 20,
                        lastEngineState.isEngineStarted
                            ? "Engine: ON"
                            : "Engine: OFF"
                    );
                }
            }

            void redrawDistanceCalibrationValue(const Configuration &configuration) {
                if (lastDistanceCalibrationValue.error != 0) {
                    display.drawStr(0, 10, ("Stop at: ERR" + String((unsigned short)lastEngineState.error)).c_str());
                } else {
                    const unsigned short stopDistance = configuration.stopDistance + lastDistanceCalibrationValue.value;
                    display.drawStr(0, 10, ("Stop at: " + String(stopDistance) + "cm").c_str());
                }
            }

            void redrawBlinkingLevel() {
                if (lastHighlighSetResult.error != 0) {
                    display.drawStr(0, 30, ("State: ERR" + String((unsigned short)lastHighlighSetResult.error)).c_str());
                } else {
                    display.drawStr(0, 30, ("State: " + String(getBlinkingLevelAsString(lastHighlighSetResult.level))).c_str());
                }
            }

            void redrawLightState() {
                if (lastHighlighSetResult.error != 0) {
                    display.drawStr(100, 30, "[E]");
                } else {
                    display.drawStr(100, 30, lastHighlighSetResult.actualState == true ? "[*]" : "[  ]");
                }
            }

            void redrawDisplayInfo(const Configuration &configuration) {
                display.clearBuffer();

                redrawDistanceCalibrationValue(configuration);
                redrawDistance();
                redrawEngineState();
                redrawBlinkingLevel();
                redrawLightState();

                display.sendBuffer();
            }

            char* getBlinkingLevelAsString(const BlinkingLevel &level) {
                switch (level) {
                    case BlinkingLevel::OFF: return "OFF";
                    case BlinkingLevel::NEAR: return "+";
                    case BlinkingLevel::MEDIUM: return "++";
                    case BlinkingLevel::FAR: return "+++";
                    default: return "###";
                }
            }

        public:
            SSD1306_StateIndicator():
                display(U8G2_R0, /* reset: */ U8X8_PIN_NONE) {
                    display.begin();
                    display.clearBuffer();
                    display.setFont(u8g2_font_ncenB08_tr);
                    display.drawStr(0, 20, "Initializing program...");
                    display.sendBuffer();
                }

            void update(
                const Configuration &configuration,
                const Distance &distance,
                const EngineState &engineState,
                const DistanceCalibrationValue &calibrationValue,
                const HighlightSetResult &highlightSetResult
            ) override {
                const bool updateNeeded = displayUpdateNeeded(
                    configuration,
                    distance,
                    engineState,
                    calibrationValue,
                    highlightSetResult
                );

                if (updateNeeded) {
                    lastDistance = distance;
                    lastEngineState = engineState;
                    lastDistanceCalibrationValue = calibrationValue;
                    lastHighlighSetResult = highlightSetResult;

                    redrawDisplayInfo(configuration);
                }
            }
    };
}