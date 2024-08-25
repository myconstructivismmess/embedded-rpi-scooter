// Standard includes
//#include <iostream>

// External includes
#include "Color.hpp"
//#include "VisualScooterControlProgram.h"

// External libraries includes
#include <ws2811.h>

// Constants
#define TURN_LEFT_SIGNAL_BUTTON_PIN 4
#define TURN_RIGHT_SIGNAL_BUTTON_PIN 5
#define LIGHT_ON_OFF_BUTTON_PIN 6
#define BRAKE_DETECTOR_PIN 13

#define FRONT_LEDS_PIN 18
#define BACK_LEDS_PIN 12

#define TURN_SIGNALS_COLOR 0xFFA500 // 255, 165, 0

// Led Constants
#define TARGET_FREQ WS2811_TARGET_FREQ

// Main function
int main()
{
    const Color turnSignalColor = Color(TURN_SIGNALS_COLOR);
    // VisualScooterControlProgramPins pins = {
    //     TURN_LEFT_SIGNAL_BUTTON_PIN,
    //     TURN_RIGHT_SIGNAL_BUTTON_PIN,
    //     LIGHT_ON_OFF_BUTTON_PIN,
    //     BRAKE_DETECTOR_PIN,
    //     FRONT_LEDS_PIN,
    //     BACK_LEDS_PIN
    // };

    // VisualScooterControlProgram program(
    //     pins
    // );

    // VisualScooterControlProgram::registerSignalHandlers();
    // program.runUntilStopped();

    return 0;
}
