#pragma once

namespace Devices::HighlightedSign {
    enum BlinkingLevel {
        STOP = 0,
        NEAR = 1,
        MEDIUM = 2,
        FAR = 3,
        OFF = 4
    };
}

namespace Devices::HighlightedSign::Abstract {
    class IHighlightedSign {
        public:
            virtual unsigned char setBlinkingLevel(BlinkingLevel level) = 0;
    };
}