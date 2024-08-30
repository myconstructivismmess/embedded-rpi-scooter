// Standard includes
#include <vector>
using std::vector;

// External libraries includes
#include <ws2811.h>

// External includes
#include "VisualScooterControlProgram.hpp"

// Constants
#define TURN_LEFT_SIGNAL_BUTTON_PIN  4
#define TURN_RIGHT_SIGNAL_BUTTON_PIN 5
#define LIGHT_ON_OFF_BUTTON_PIN      6
#define BRAKE_DETECTOR_PIN           13

#define FRONT_LEDS_PIN               18
#define FRONT_LEDS_COUNT             60
#define FRONT_LEDS_BRIGHTNESS        255


#define BACK_LEDS_PIN                12
#define BACK_LEDS_COUNT              60
#define BACK_LEDS_BRIGHTNESS         255

#define TURN_SIGNALS_COLOR           0xFFA500 // 255, 165, 0

// Led Constants
#define LEDS_TARGET_FREQUENCY WS2811_TARGET_FREQ

// Main function
int main() {
    ws2811_t ledStripsProperties = {
        .freq = LEDS_TARGET_FREQUENCY,
        .dmanum = 10,
        .channel = {
            [0] =
            {
                .gpionum = BACK_LEDS_PIN, // FRONT_LEDS_PIN,
                .invert = 0,
                .count = FRONT_LEDS_COUNT,
                .strip_type = WS2811_STRIP_GBR,
                .brightness = FRONT_LEDS_BRIGHTNESS,
            }//,
            // [1] =
            // {
            //     .gpionum = BACK_LEDS_PIN,
            //     .invert = 0,
            //     .count = BACK_LEDS_COUNT,
            //     .strip_type = WS2811_STRIP_GBR,
            //     .brightness = BACK_LEDS_BRIGHTNESS,
            // },
        },
    };
    vector<size_t> ledStripsLedCount = { FRONT_LEDS_COUNT };
    // vector<size_t> ledStripsLedCount = { FRONT_LEDS_COUNT, BACK_LEDS_COUNT };

    VisualScooterControlProgram program(
        VisualScooterControlProgram::Pins {
            turnLeftSignalButtonPin: TURN_LEFT_SIGNAL_BUTTON_PIN,
            turnRightSignalButtonPin: TURN_RIGHT_SIGNAL_BUTTON_PIN,
            lightOnOffButtonPin: LIGHT_ON_OFF_BUTTON_PIN,
            brakeDetectorPin: BRAKE_DETECTOR_PIN,
            frontLedsPin: FRONT_LEDS_PIN,
            backLedsPin: BACK_LEDS_PIN
        },
        VisualScooterControlProgram::Properties {
            turnSignalColor: TURN_SIGNALS_COLOR,
            ledStripsPtr: &ledStripsProperties,
            ledStripsLedCount: ledStripsLedCount,
        }
    );

    program.registerSignalHandlers();
    program.runUntilStopped();

    return 0;
}
