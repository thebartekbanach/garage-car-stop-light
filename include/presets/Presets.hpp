#pragma once

/*
*   NOTE: DO NOT INCLUDE MORE THAN ONE PRESET
*
*   Only one preset can be included!
*   You will got compilation errors
*   when more than one preset is included.
*
*   If you want to include selected preset
*   just uncomment selected #include directive
*   and make sure you have commented
*   the rest of includes.
*/


/*
*   This setup uses following devices:
*   - HC-SR04 ultrasonic distance sensor as distance sensor
*   - FC-04 sound sensor as car engine state sensor
*   - Potentiometer as a stop distance calibrator
*   - Default driver of highlighted sign
*   - SSD1306 display as a state indicator
*
*   Default pinout:
*   = HC-SR04 ultrasonic distance sensor
*       - trigger pin -> PB10
*       - echo pin -> PB11
*   = FC-04 sound sensor
*       - data pin -> PB4
*   = Distance calibration potentiometer (as 3.3V - 0V voltage divider)
*       - divider output -> PA7
*   = Highlight switcher module
*       - highlight steering pin -> PB13
*       - highlight turned on electric pin state -> LOW
*   = SSD1306 display
*       - SDA -> PB7
*       - SCL -> PB6
*
*   Configuration is available inside
*   "stm32_blue_pill/Preset_01.hpp" file.
*/

#include "stm32_blue_pill/Preset_01.hpp"



/*
*   This setup uses following devices:
*   - HC-SR04 ultrasonic distance sensor as distance sensor
*   - FC-04 sound sensor as car engine state sensor
*   - Potentiometer as a stop distance calibrator
*   - Default driver of highlighted sign
*   - without state indicator
*
*   Default pinout:
*   = HC-SR04 ultrasonic distance sensor
*       - trigger pin -> PB10
*       - echo pin -> PB11
*   = FC-04 sound sensor
*       - data pin -> PB4
*   = Distance calibration potentiometer (as 3.3V - 0V voltage divider)
*       - divider output -> PA7
*   = Highlight switcher module
*       - highlight steering pin -> PB13
*       - highlight turned on electric pin state -> LOW
*
*   Configuration is available inside
*   "stm32_blue_pill/Preset_02.hpp" file.
*/

// #include "stm32_blue_pill/Preset_02.hpp"