#pragma once

#include "devices/abstract/DistanceCalibrator.hpp"
#include "devices/abstract/CarEngineStateSensor.hpp"
#include "devices/abstract/DistanceSensor.hpp"
#include "devices/abstract/HighlightedSign.hpp"
#include "devices/abstract/StateIndicator.hpp"

#include "Configuration.hpp"

using Devices::DistanceCalibrator::Abstract::IDistanceCalibrator;
using Devices::CarEngineStateSensor::Abstract::ICarEngineStateSensor;
using Devices::DistanceSensor::Abstract::IDistanceSensor;
using Devices::HighlightedSign::Abstract::IHighlightedSign;
using Devices::StateIndicator::Abstract::IStateIndicator;

using Devices::HighlightedSign::BlinkingLevel;

namespace Core {
    class Program {
        private:
            IDistanceCalibrator* distanceCalibrator;
            ICarEngineStateSensor* carEngineSensor;
            IDistanceSensor* distanceSensor;
            IHighlightedSign* highlightedSing;
            IStateIndicator* stateIndicator;

            Configuration* config;

            BlinkingLevel getBlinkingLevelFor(const float& distance, const short& calibrationValue) {
                if (distance > config->farDistance) {
                    return BlinkingLevel::OFF;
                }

                else if (distance <= (config->stopDistance + calibrationValue)) {
                    return BlinkingLevel::STOP;
                }
                
                else if (distance <= config->nearDistance) {
                    return BlinkingLevel::NEAR;
                }

                else if (distance <= config->mediumDistance) {
                    return BlinkingLevel::MEDIUM;
                }

                else if (distance <= config->farDistance) {
                    return BlinkingLevel::FAR;
                }
            }

        public:
            Program(
                Configuration* _config,
                IDistanceCalibrator* _distanceCalibrator,
                ICarEngineStateSensor* _carEngineSensor,
                IDistanceSensor* _distanceSensor,
                IHighlightedSign* _highlightedSing,
                IStateIndicator* _stateIndicator
            ):
                config(_config),
                distanceCalibrator(_distanceCalibrator),
                carEngineSensor(_carEngineSensor),
                distanceSensor(_distanceSensor),
                highlightedSing(_highlightedSing),
                stateIndicator(_stateIndicator)
            {}

            void update() {
                const auto& engineState = carEngineSensor->sense();
                const auto& distance = distanceSensor->measure();
                const auto& calibrationValue = distanceCalibrator->getValue();

                bool emergencyMode = engineState.error || distance.error || calibrationValue.error;

                BlinkingLevel blinkingLevel;

                if (emergencyMode) {
                    blinkingLevel = BlinkingLevel::STOP;
                }
                
                else if (!engineState.isEngineStarted || distance.tooFar) {
                    blinkingLevel = BlinkingLevel::OFF;
                }
                
                else {
                    blinkingLevel = getBlinkingLevelFor(distance.distance, calibrationValue.value);
                }

                const unsigned char blinkingLevelSetResult = highlightedSing->setBlinkingLevel(blinkingLevel);

                stateIndicator->update(
                    *config,
                    distance,
                    engineState,
                    calibrationValue,
                    blinkingLevel,
                    blinkingLevelSetResult
                );
            }
    };
}