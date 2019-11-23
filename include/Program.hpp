#pragma once

#include "devices/abstract/CalibrationPotentiometer.hpp"
#include "devices/abstract/CarEngineStateSensor.hpp"
#include "devices/abstract/DistanceSensor.hpp"
#include "devices/abstract/HighlightedSign.hpp"
#include "devices/abstract/StateDiode.hpp"

#include "Configuration.hpp"

typedef Devices::CalibrationPotentiometer::Abstract::ICalibrationPotentiometer  ICalibrationPotentiometer;
typedef Devices::CarEngineStateSensor::Abstract::ICarEngineStateSensor          ICarEngineStateSensor;
typedef Devices::DistanceSensor::Abstract::IDistanceSensor                      IDistanceSensor;
typedef Devices::HighlightedSign::Abstract::IHighlightedSign                    IHighlightedSign;
typedef Devices::StateDiode::Abstract::IStateDiode                              IStateDiode;

class Program {
    private:
        ICalibrationPotentiometer* calibrationPotentiometr;
        ICarEngineStateSensor* carEngineSensor;
        IDistanceSensor* distanceSensor;
        IHighlightedSign* highlightedSing;
        IStateDiode* stateDiode;

        Configuration* config;

        bool emergencyMode = false;

    public:
        Program(
            Configuration* _config,
            ICalibrationPotentiometer* _calibrationPotentiometr,
            ICarEngineStateSensor* _carEngineSensor,
            IDistanceSensor* _distanceSensor,
            IHighlightedSign* _highlightedSing,
            IStateDiode* _stateDiode
        ):
            config(_config),
            calibrationPotentiometr(_calibrationPotentiometr),
            carEngineSensor(_carEngineSensor),
            distanceSensor(_distanceSensor),
            highlightedSing(_highlightedSing),
            stateDiode(_stateDiode)
        {}

        void update() {
            if (emergencyMode) {
                stateDiode->setState(Devices::StateDiode::DriverState::ERROR);
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::STOP);
                return;
            }

            stateDiode->setState(Devices::StateDiode::DriverState::OK);

            const auto& engineState = carEngineSensor->sense();

            if (engineState.error) {
                emergencyMode = true;
                return;
            }

            if (!engineState.isEngineStarted) {
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::OFF);
                return;
            }

            const auto& distance = distanceSensor->measure();

            if (distance.error) {
                emergencyMode = true;
                return;
            }

            const auto& calibrationValue = calibrationPotentiometr->getValue();

            if (calibrationValue.error) {
                emergencyMode = true;
                return;
            }

            if (distance.tooFar || distance.distance > config->farDistance) {
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::OFF);
            }

            else if (distance.distance <= (config->stopDistance + calibrationValue.value)) {
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::STOP);
            }
            
            else if (distance.distance <= config->nearDistance) {
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::NEAR);
            }

            else if (distance.distance <= config->mediumDistance) {
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::MEDIUM);
            }

            else if (distance.distance <= config->farDistance) {
                highlightedSing->setBlinkingLevel(Devices::HighlightedSign::BlinkingLevel::FAR);
            }
        }
};