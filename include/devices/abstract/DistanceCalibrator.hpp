#pragma once

namespace Devices::DistanceCalibrator {
    struct DistanceCalibrationValue {
        short value; // in centimeters
        unsigned char error;
    };
}

namespace Devices::DistanceCalibrator::Abstract {
    class IDistanceCalibrator {
        public:
            virtual const DistanceCalibrationValue& getValue() = 0;
    };
}