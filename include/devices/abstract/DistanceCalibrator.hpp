#pragma once

namespace Devices::DistanceCalibrator {
    struct DistanceCalibrationValue {
        short value = 0; // in centimeters
        unsigned char error = 0;
    };
}

namespace Devices::DistanceCalibrator::Abstract {
    class IDistanceCalibrator {
        public:
            virtual const DistanceCalibrationValue& getValue() = 0;
    };
}