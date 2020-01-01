# Garage parking assistant

### Language: **English** / [Polski](./README.pl.md)

Garage parking assistant is a simple engine
which we upload to our target controller.

## Main goals of this project:
- a main engine that is portable between different platforms
- simple substitution and selection of components that we want to use in the project
- simple three-level indication of distance from the wall
- detecting errors and warning users about the error before they starts to park
- switching on the distance indicator only when it is detecting that the car engine is running

## How it works
The device activates only when it detects the sound of our car's engine.
When active, it shows us the current distance between the car and the garage wall.
Distance indication consists of flashing with a led beam (STOP sign) in various ways,
depending on the distance. For example, at a distance of 2m from the wall
our beam blinks once a second, one meter away from the wall
the beam blinks twice a second and at a distance of 50cm the beam blinks 5 times a second.
When we pass a certain point, for example 10cm from the wall, the beam stops flashing
and shines steadily. The STOP point can be adjusted with a potentiometer.

Way of indicating the distance, calibration of the STOP point, way of taking the distance,
how to detect if the car engine is on and the status indicator can be changed by
choosing another preset, creating your own preset or writing your own device driver
if it is not available yet.

## Mounting method
Our parking assistant should be mounted on the wall opposite the garage entrance.
The distance sensor and the engine activation sensor must be facing the car
and be placed at the height of the license plate of our car.
The STOP LED bar should be mounted at eye level of the car driver.

## Presets
To simplify the programming process of your own controller for people who
don't know how to code, the project contains so-called **presets**.

**Presets** are predefined sets that work with selected sensors just simply
after uploading it to the controller. They make easy to program your own assistant
requiring only connecting sensors and other devices to selected pins
controller, making the whole construction process much faster.

The choice of which preset to use depends on what board
we have and which sensors we want to connect.

Available presets are located in the `include/presets/` directory.
All subfolders in this path are the types of controllers for which the presets
are written. For example, in the `include/presets/stm32_blue_pill` directory
are placed presets written for the `stm32 blue pill` board.

To select a given preset, all you need is to go to the
`include/presets/Presets.hpp` file and uncomment
selected line to unlock the your preset.

If you need to change the pinout, just enter the configuration file of
the preset you choosed and edit it contents, but in most cases
this should not be needed.

## Available presets:
- `stm32_blue_pill/Preset_01` - preset for the `stm32 blue pill` board. To measure distances, it uses a cheap and widely available ultrasonic distance sensor `HC-SR04`. The `FC-04` sound sensor is used as the engine state detection sensor, whose sensitivity have to be calibrated to detect the noise of our car's engine. Calibration of the STOP position is done by voltage divider built of a potentiometer, its maximum voltage is 3.3V and the minimum is 0V. Controlling the STOP sign is done by a simple driver that gives us a high or low state (depending on the configuration) to the pin provided in the configuration when the lighting is on and its opposite when the lighting is off. Using this module is very simple, just connect the STOP sign lighting transistor to the configured pin of the board. This preset has no device status indicator module configured.

- `stm32_blue_pill/Preset_02` - the preset contains the same configuration as `Preset_01`. The difference is the device status indicator as which the `SSD1306` display connected via the I2C bus is used.

## Simple device diagnostics
Simple mini diagnostic projects of devices are available in the project
in the `include/diagnostics` directory, which is structured in the style <br />
`include/diagnostics/<platform>/<diagnostics_name>`.

To unlock a given diagnostic project, change the value of the first one
`#define` directive to `true`. For example, for a diagnostic project
available in the `include/diagnostics/stm32/BlinkExample.hpp` file
it will be line number 3: <br />
`#define BLINK_DIAGNOSTICS_ENABLED false` <br />
whose value needs to be changed as follows: <br />
`#define BLINK_DIAGNOSTICS_ENABLED true`

Then we upload our project to our board.

We proceed in a similar way in other diagnostic projects.

# The code

## Program structure
The main program core that retrieves data and updates the other modules is located
in the `include/core/Program.hpp` file.

The `Core::Program` class uses five external dependencies.
These dependencies are injected by the constructor. They are abstractions
on various external devices and can be found in the folder
`include/devices/abstract`.

## Our dependencies are:
- `Devices::DistanceSensor::Abstract::IDistanceSensor` - this is the distance sensor interface. It has one method `const Distance meansure()` which measures and returns the distance to the car. The return type `Devices::DistanceSensor::Distance` has three properties:
    - `float distance` - containing the distance between the car and the sensor
    - `bool tooFar` - is set to `true` when the distance between the car and the sensor is too large, so the sensor cannot give the correct distance
    - `unsigned char error` - if the sensor driver detects an error, this variable is set to a value other than 0. When the value is other than 0, it will be displayed on the screen if it is possible.

- `Devices::CarEngineStateSensor::Abstract::ICarEngineState` - engine state sensor interface. The `const EngineState sense()` method checks if the engine is turned on and returns the result as the `Devices::CarEngineStateSensor::EngineState` structure which contains two properties:
    - `bool isEngineStarted` - set to `true` when the engine is running
    - `unsigned char error` - if it is set to something other than zero, it means that an error was ocurred. The error code will be displayed if it is possible.

- `Devices::DistanceCalibrator::Abstract::IDistanceCalibrator` - interface of a potentiometer or other module that sets the advance or delay of lighting of STOP lighting. The program uses the `const DistanceCalibrationValue getValue()` method to retrieve this value. The `Devices::DistanceCalibrator::DistanceCalibrationValue` structure has two properties:
    - `short value` - calibration value in centimeters
    - `unsigned char error` - if set to a value other than zero, it means that an error was detected. The error code will be displayed if possible.

- `Devices::HighlightedSign::Abstract::IHighlightedSign` - interface of the highlighted STOP sign. Includes the `setBlinkingLevel` method to set the current sign blinking mode. The method returns an object of type `enum Devices::HighlightedSign::Blinkinglevel` and takes two arguments:
    - `const BlinkingLevel &level` - current lighting mode
    - `const unsigned short &distanceInCm` - current distance in centimeters.

    The `Devices::HighlightedSign::HighlightSetResult` structure has three properties:
    - `enum Devices::HighlightedSign::Blinkinglevel level` - the current blinking mode, which can take one of five values:
        - `STOP` - sign for the driver to stop
        - `NEAR` - a flashing mode telling the driver that he is about to stop
        - `MEDIUM` - means you should be prepared for braking
        - `FAR` - mainly serves to show the driver that the parking assistant is active and functioning correctly
        - `OFF` - turns off the flashing, the car may be too far away or be turned off
    - `bool actualState` - the current state of the lights, whether they are lit or not
    - `unsigned char error` - if set to a value other than zero, it means that an error was detected. The error code will be displayed if possible.

- `Devices::StateIndicator::Abstract::IStateIndicator` - controller state indicator interface. It contains only one method `void update(...)`. The arguments given to this function are the latest data got from the sensors and the results of the modules execution. That module can display this data on display or print it to serial.

## Implementations of device abstraction
Each interface implementation must implement all methods of the interface.
Implementation examples can be found in the `devices/implementation` folder.
It is divided into subfolders, one for each platform, and then for abstract type.
Below in each folder you will find real implementations of sensor and other device abstractions.

In short, the implementation directories look like this: <br />
`devices/implementation/<platform>/<sensor_type>/<sensor_name>` <br />
For example: <br />
`devices/implementation/stm32/distanceSensor/HC_SR04` <br />

## Contributions
Contributions are welcome.
If you create a parking assistant based on your own components
it would be awesome to share your work to other people by making pull request to this repo.
Just remember to use namespaces, maintain code in good quality
and write proper documentation of presets you created.

## License
The license is available in the repository in the `LICENSE` file.

