#pragma once

namespace Devices::CarEngineStateSensor {
    struct EngineState {
        bool isEngineStarted = false;
        unsigned char error = 0;
    };
}

namespace Devices::CarEngineStateSensor::Abstract {
    class ICarEngineStateSensor {
        public:
            virtual const EngineState& sense() = 0;
    };
}