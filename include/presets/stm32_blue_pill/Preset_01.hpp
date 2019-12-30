#pragma once

/*
*   This setup uses following devices:
*   - HC-SR04 ultrasonic distance sensor as distance sensor
*   - FC-04 sound sensor as car engine state sensor
*   - Potentiometer as a stop distance calibrator
*   - Default driver of highlighted sign
*/

// HC-SR04 ultrasonic distance sensor configuration
#define HC_SR04_TRIGGER_PIN PB10
#define HC_SR04_ECHO_PIN PB11

// FC-04 sound sensor configuration
#define FC_04_DATA_PIN PB4

// Distance calibration potentiometer configuration
#define POTENTIOMETER_OUTPUT_PIN PA7

// Highlight switch transistor settings
#define HIGHLIGHT_STEERING_PIN PB13
#define HIGHLIGHT_TURNED_ON_ELECTRIC_STATE LOW

// Distnaces setup
#define STOP_DISTANCE 5
#define NEAR_DISTANCE 30
#define MEDIUM_DISTANCE 80
#define FAR_DISTANCE 150



#include <Arduino.h>

#include "../../devices/implementation/stm32/distanceSensor/HC_SR04/Sensor.hpp"
#include "../../devices/implementation/stm32/carEngineStateSensor/FC_04/Sensor.hpp"
#include "../../devices/implementation/stm32/distanceCalibrator/Potentiometer/DistanceCalibrator.hpp"
#include "../../devices/implementation/stm32/highlightedSign/Default_driver/Driver.hpp"
#include "../../devices/implementation/stm32/stateIndicator/SSD1306/StateIndicator.hpp"

#include "../../Core/Program.hpp"

using namespace Devices;

DistanceSensor::Abstract::IDistanceSensor *distanceSensor = nullptr;
CarEngineStateSensor::Abstract::ICarEngineStateSensor *carEngineStateSensor = nullptr;
DistanceCalibrator::Abstract::IDistanceCalibrator *distanceCalibrator = nullptr;
HighlightedSign::Abstract::IHighlightedSign *highlightedSign = nullptr;
StateIndicator::Abstract::IStateIndicator *stateIndicator = nullptr;

Core::Program *program = nullptr;
    
void setup() {
    auto *distanceSensorConfig = new DistanceSensor::Implementation::Stm32::HC_SR04::HC_SR04_Configuration();
    distanceSensorConfig->triggerPin = HC_SR04_TRIGGER_PIN;
    distanceSensorConfig->echoPin = HC_SR04_ECHO_PIN;

    auto *carEngineSensorConfig = new CarEngineStateSensor::Implementation::Stm32::FC_04::FC_04_Configuration();
    carEngineSensorConfig->sensorDataInputPin = FC_04_DATA_PIN;

    auto *distanceCalibratorConfig = new DistanceCalibrator::Implementation::Stm32::Potentiometer::Potentiometer_Configuration();
    distanceCalibratorConfig->potentiometerDataInputPin = POTENTIOMETER_OUTPUT_PIN;
    distanceCalibratorConfig->maximumAdditionValueOfPotentiometer = 5;
    distanceCalibratorConfig->minimumAdditionValueOfPotentiometer = -2;

    auto *highlightedSignConfig = new HighlightedSign::Implementation::Stm32::Default_driver::DefaultDriver_Configuration();
    highlightedSignConfig->highlightPinWiringMode = WiringPinMode::OUTPUT;
    highlightedSignConfig->highlightSteeringPin = HIGHLIGHT_STEERING_PIN;
    highlightedSignConfig->turnedOnElectricState = HIGHLIGHT_TURNED_ON_ELECTRIC_STATE;
    highlightedSignConfig->mirrorBlinkingOnLedBuiltin = true;
    highlightedSignConfig->lightingSchemes = {
        .nearLightingScheme = {
            .onTime = 300,
            .offTime = 200
        },
        .mediumLightingScheme = {
            .onTime = 300,
            .offTime = 800
        },
        .farLightingScheme = {
            .onTime = 400,
            .offTime = 1400
        }
    };

    distanceSensor = new DistanceSensor::Implementation::Stm32::HC_SR04::HC_SR04_DistanceSensor(*distanceSensorConfig);
    carEngineStateSensor = new CarEngineStateSensor::Implementation::Stm32::FC_04::FC_04_CarEngineStateSensor(*carEngineSensorConfig);
    distanceCalibrator = new DistanceCalibrator::Implementation::Stm32::Potentiometer::Potentiometer_DistanceCalibrator(*distanceCalibratorConfig);
    highlightedSign = new HighlightedSign::Implementation::Stm32::Default_driver::DefaultDriver_HiglightedSign(*highlightedSignConfig);
    stateIndicator = new StateIndicator::Implementation::Stm32::SSD1306::SSD1306_StateIndicator();

    Core::Configuration *programConfig = new Core::Configuration();
    programConfig->stopDistance = STOP_DISTANCE;
    programConfig->nearDistance = NEAR_DISTANCE;
    programConfig->mediumDistance = MEDIUM_DISTANCE;
    programConfig->farDistance = FAR_DISTANCE;

    program = new Core::Program(
        programConfig,
        distanceCalibrator,
        carEngineStateSensor,
        distanceSensor,
        highlightedSign,
        stateIndicator
    );
}

void loop() {
    program->update();
}