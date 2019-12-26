#pragma once
#include "DistanceCalibrator.hpp"
#include "CarEngineStateSensor.hpp"
#include "DistanceSensor.hpp"
#include "HighlightedSign.hpp"

#include "../../Configuration.hpp"

using Devices::DistanceSensor::Distance;
using Devices::CarEngineStateSensor::EngineState;
using Devices::DistanceCalibrator::DistanceCalibrationValue;
using Devices::HighlightedSign::BlinkingLevel;

using Core::Configuration;

namespace Devices::StateIndicator::Abstract {
    class IStateIndicator {
        public:
            virtual void update(
                const Configuration &configuration,
                const Distance &distance,
                const EngineState &engineState,
                const DistanceCalibrationValue &value,
                const BlinkingLevel &blinkingLevel,
                const unsigned char &blinkingSetResult
            ) = 0;
    };
}
