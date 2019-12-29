#pragma once

/*
*   This setup uses following devices:
*   - HC-SR04 ultrasonic distance sensor as distance sensor
*   - FC-04 sound sensor as car engine state sensor
*   - Potentiometer as a stop distance calibrator
*   - Default driver of highlighted sign
*/
#define USE_SETUP_1
#ifdef USE_SETUP_1
#include <Arduino.h>

#include "../distanceSensor/HC_SR04/Sensor.hpp"
#include "../carEngineStateSensor/FC_04/Sensor.hpp"
#include "../distanceCalibrator/Potentiometer/DistanceCalibrator.hpp"
#include "../highlightedSign/Default_driver/Driver.hpp"
#include "../stateIndicator/SSD1306/StateIndicator.hpp"

#include "../../../../Program.hpp"

using namespace Devices;

DistanceSensor::Abstract::IDistanceSensor *distanceSensor = nullptr;
CarEngineStateSensor::Abstract::ICarEngineStateSensor *carEngineStateSensor = nullptr;
DistanceCalibrator::Abstract::IDistanceCalibrator *distanceCalibrator = nullptr;
HighlightedSign::Abstract::IHighlightedSign *highlightedSign = nullptr;
StateIndicator::Abstract::IStateIndicator *stateIndicator = nullptr;

Core::Program *program = nullptr;
    
void setup() {
    auto *distanceSensorConfig = new DistanceSensor::Implementation::Stm32::HC_SR04::HC_SR04_Configuration();
    distanceSensorConfig->triggerPin = PB10;
    distanceSensorConfig->echoPin = PB11;

    auto *carEngineSensorConfig = new CarEngineStateSensor::Implementation::Stm32::FC_04::FC_04_Configuration();
    carEngineSensorConfig->sensorDataInputPin = PB4;

    auto *distanceCalibratorConfig = new DistanceCalibrator::Implementation::Stm32::Potentiometer::Potentiometer_Configuration();
    distanceCalibratorConfig->potentiometerDataInputPin = PA7;
    distanceCalibratorConfig->maximumAdditionValueOfPotentiometer = 5;
    distanceCalibratorConfig->minimumAdditionValueOfPotentiometer = -3;

    auto *highlightedSignConfig = new HighlightedSign::Implementation::Stm32::Default_driver::DefaultDriver_Configuration();
    highlightedSignConfig->highlightPinWiringMode = WiringPinMode::OUTPUT;
    highlightedSignConfig->highlightSteeringPin = PB13;
    highlightedSignConfig->turnedOnElectricState = LOW;
    highlightedSignConfig->mirrorBlinkingOnLedBuiltin = true;
    highlightedSignConfig->lightingSchemes = {
        .nearLightingScheme = {
            .onTime = 300,
            .offTime = 300
        },
        .mediumLightingScheme = {
            .onTime = 300,
            .offTime = 800
        },
        .farLightingScheme = {
            .onTime = 300,
            .offTime = 1400
        }
    };

    distanceSensor = new DistanceSensor::Implementation::Stm32::HC_SR04::HC_SR04_DistanceSensor(*distanceSensorConfig);
    carEngineStateSensor = new CarEngineStateSensor::Implementation::Stm32::FC_04::FC_04_CarEngineStateSensor(*carEngineSensorConfig);
    distanceCalibrator = new DistanceCalibrator::Implementation::Stm32::Potentiometer::Potentiometer_DistanceCalibrator(*distanceCalibratorConfig);
    highlightedSign = new HighlightedSign::Implementation::Stm32::Default_driver::DefaultDriver_HiglightedSign(*highlightedSignConfig);
    stateIndicator = new StateIndicator::Implementation::Stm32::SSD1306::SSD1306_StateIndicator();

    Core::Configuration *programConfig = new Core::Configuration();
    programConfig->stopDistance = 5;

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

#endif