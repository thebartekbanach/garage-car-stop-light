#pragma once
#include <Arduino.h>

#include "../../../../abstract/DistanceCalibrator.hpp"

#include "Configuration.hpp"

namespace Devices::DistanceCalibrator::Implementation::Stm32::Potentiometer {
    class Potentiometer_DistanceCalibrator: public Abstract::IDistanceCalibrator {
        private:
            const Potentiometer_Configuration &config;
        public:
            Potentiometer_DistanceCalibrator(const Potentiometer_Configuration &_configuration):
                config(_configuration) {
                    pinMode(config.potentiometerDataInputPin, INPUT_ANALOG);
                }

            const DistanceCalibrationValue getValue() override {
                const unsigned short value = analogRead(config.potentiometerDataInputPin);
                const short result = map(
                    value,
                    10, 4000,
                    config.minimumAdditionValueOfPotentiometer,
                    config.maximumAdditionValueOfPotentiometer
                );
                
                return {
                    .value = result,
                    .error = (unsigned char) 0
                };
            }
    };
}