#pragma once

namespace Devices::HighlightedSign {
    enum BlinkingLevel {
        STOP = 0,
        NEAR = 1,
        MEDIUM = 2,
        FAR = 3,
        OFF = 4
    };

    struct HighlightSetResult {
        BlinkingLevel level;
        bool actualState;
        unsigned char error;
    };
}

namespace Devices::HighlightedSign::Abstract {
    class IHighlightedSign {
        public:
            virtual const HighlightSetResult setBlinkingLevel(const BlinkingLevel &level, const unsigned short &distanceInCm = 0) = 0;
    };
}