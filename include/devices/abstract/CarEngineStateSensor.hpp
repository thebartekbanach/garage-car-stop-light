#pragma once

namespace Devices::CarEngineStateSensor {
    struct EngineState {
        bool isEngineStarted = false;
        bool error = false;
    };
}

namespace Devices::CarEngineStateSensor::Abstract {
    class ICarEngineStateSensor {
        public:
            virtual const EngineState& sense() = 0;
    };
}