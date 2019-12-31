#pragma once

namespace Devices::CarEngineStateSensor::Implementation::Stm32::FC_04 {
    struct FC_04_Configuration {
        unsigned short sensorDataInputPin;
        unsigned short numberOfSensesPerMeansurement = 1500;
        unsigned short minimumNumberOfPositiveSensesToTellThatEngineIsOn = 10;
    };
}