#pragma once
#include <Arduino.h>

#include "../../../../abstract/CarEngineStateSensor.hpp"

#include "Configuration.hpp"

namespace Devices::CarEngineStateSensor::Implementation::Stm32::FC_04 {
    class FC_04_CarEngineStateSensor: public Abstract::ICarEngineStateSensor {
        private:
            const FC_04_Configuration &config;

        public:
            FC_04_CarEngineStateSensor(const FC_04_Configuration &configuration):
                config(configuration) {
                    pinMode(configuration.sensorDataInputPin, INPUT_PULLUP);
                }

            const EngineState& sense() override {
                unsigned short numberOfPositiveDetections = 0;

                for (unsigned short i = 0; i < config.numberOfSensesPerMeansurement; ++i) {
                    numberOfPositiveDetections += !digitalRead(config.sensorDataInputPin);
                }

                return EngineState {
                    .isEngineStarted =
                        numberOfPositiveDetections > config.minimumNumberOfPositiveSensesToTellThatEngineIsOn,
                    .error = 0
                };
            }
    };
}