#pragma once

namespace Devices::CalibrationPotentiometer {
    struct CalibrationPotetniometerValue {
        short value = 0; // in centimeters
        bool error = false;
    };
}

namespace Devices::CalibrationPotentiometer::Abstract {
    class ICalibrationPotentiometer {
        public:
            virtual const CalibrationPotetniometerValue& getValue() = 0;
    };
}