#pragma once

namespace Devices::DistanceCalibrator::Implementation::Stm32::Potentiometer {
    struct Potentiometer_Configuration {
        unsigned short potentiometerDataInputPin;
        unsigned short maximumAdditionValueOfPotentiometer;
        short minimumAdditionValueOfPotentiometer;
    };
}