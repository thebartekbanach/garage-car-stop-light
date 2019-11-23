#pragma once

namespace Devices::StateDiode {
    enum DriverState {
        OFF = 0,
        ERROR = 1,
        OK = 2
    };
}

namespace Devices::StateDiode::Abstract {
    class IStateDiode {
        public:
            virtual void setState(DriverState state) = 0;
    };
}

