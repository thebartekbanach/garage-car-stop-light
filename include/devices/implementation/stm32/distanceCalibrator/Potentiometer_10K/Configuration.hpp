#pragma once

namespace Devices::DistanceCalibrator::Implementation::Stm32::Potentiometer_10K {
    struct Potentiometer_10K_Configuration {
        unsigned int potentiometerDataInputPin;
        unsigned int maximumAdditionValueOfPotentiometer;
    };
}