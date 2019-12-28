#pragma once

namespace Devices::DistanceSensor {
    struct Distance {
        float distance; // in centimeters
        bool tooFar; // when car is too far should be set to true
        unsigned char error;
    };
}

namespace Devices::DistanceSensor::Abstract {
    class IDistanceSensor {
        public:
            virtual const Distance measure() = 0;
    };
}