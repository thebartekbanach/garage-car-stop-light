#pragma once
#include <HCSR04.h>

#include "../../../../abstract/DistanceSensor.hpp"

#include "Configuration.hpp"
#include "Errors.hpp"
#include "utils/Averager.hpp"

namespace Devices::DistanceSensor::Implementation::Stm32::HC_SR04 {
    class HC_SR04_DistanceSensor: public Abstract::IDistanceSensor {
        private:
            const HC_SR04_Configuration &config;
            UltraSonicDistanceSensor sensor;

        public:
            HC_SR04_DistanceSensor(const HC_SR04_Configuration &_configuration):
                config(_configuration),
                sensor(config.triggerPin, config.echoPin) {}

            const Distance& measure() override {
                Utils::Averager averager(config.numberOfPingsPerScan);
                unsigned short errors = 0;

                for (unsigned short i = 0; i < config.numberOfPingsPerScan; ++i) {
                    const float distance = sensor.measureDistanceCm();

                    if (distance < 0) {
                        errors += 1;
                        i -= 1;

                        if (errors > config.maximumAllowedNumberOfFaultPingsPerScan) {
                            return {
                                .distance = 0,
                                .tooFar = false,
                                .error = Errors::DEVICE_FAULT
                            };
                        }

                        continue;
                    }

                    averager.addMeansurement(distance);
                }

                return {
                    .distance = averager.getAverage(),
                    .tooFar = averager.getAverage() > config.maximumAllowedDistance,
                    .error = Errors::NO_ERROR
                };
            }
    };
}