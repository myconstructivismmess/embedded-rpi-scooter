// Standard includes
#include <string>
using std::string;

// External includes
#include "AudioScooterControlProgram.hpp"

// Constants
#define HORN_SOUND_COLLECTION_PATH               "/usr/share/scootercontrol/horn/sounds/"

#define HORN_MAX_SHORT_PRESS_DURATION_SECONDS    1.0
#define HORN_MAX_PRESS_INTERVAL_DURATION_SECONDS 0.3

#define HORN_BUTTON_PIN                          26

// Main function
int main() {
    const string HORN_SOUND_COLLECTION_PATH_STR = HORN_SOUND_COLLECTION_PATH;

    AudioScooterControlProgram program(
        AudioScooterControlProgram::Pins {
            hornButtonPin: HORN_BUTTON_PIN
        },
        AudioScooterControlProgram::Properties {
            hornSoundCollectionPath: HORN_SOUND_COLLECTION_PATH_STR,
            hornMaxShortPressDurationSeconds: HORN_MAX_SHORT_PRESS_DURATION_SECONDS,
            hornMaxPressIntervalDurationSeconds: HORN_MAX_PRESS_INTERVAL_DURATION_SECONDS
        }
    );

    program.registerSignalHandlers();
    program.runUntilStopped();

    return 0;
}
