#pragma once
#include <Arduino.h>

#include "../../../../abstract/StateIndicator.hpp"

namespace Devices::StateIndicator::Implementation::Stm32::WithoutStateIndicator {
    using Devices::DistanceSensor::Distance;
    using Devices::CarEngineStateSensor::EngineState;
    using Devices::DistanceCalibrator::DistanceCalibrationValue;
    using Devices::HighlightedSign::HighlightSetResult;
    using Devices::HighlightedSign::BlinkingLevel;

    using Core::Configuration;

    class WithoutStateIndicator: public Abstract::IStateIndicator {
        public:
            void update(
                const Configuration &configuration,
                const Distance &distance,
                const EngineState &engineState,
                const DistanceCalibrationValue &value,
                const HighlightSetResult &highlightSetResult
            ) {}
    };
}