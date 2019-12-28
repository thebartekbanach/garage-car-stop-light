#pragma once

namespace Devices::CarEngineStateSensor {
    struct EngineState {
        bool isEngineStarted;
        unsigned char error;
    };
}

namespace Devices::CarEngineStateSensor::Abstract {
    class ICarEngineStateSensor {
        public:
            virtual const EngineState sense() = 0;
    };
}