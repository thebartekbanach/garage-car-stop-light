#pragma once

namespace Devices::DistanceSensor {
    struct Distance {
        unsigned short distance = 0; // in centimeters
        bool tooFar = false; // when car is too far should be set to true
        bool error = false;
    };
}

namespace Devices::DistanceSensor::Abstract {
    class IDistanceSensor {
        public:
            virtual const Distance& measure() = 0;
    };
}