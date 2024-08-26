#include "AudioScooterControlProgram.hpp"

// Standard includes
#include <memory>
using std::make_unique;

#include <vector>
using std::vector;

// Scooter control library includes
#include "button/ButtonOptionType.h"

// External includes
#include "HornSoundsLoader.hpp"

AudioScooterControlProgram::AudioScooterControlProgram(Pins pins, Properties properties)
    : Program(),
      _hornButtonHandler(HornButtonHandler::Properties {
          maxShortPressDurationSeconds: properties.hornMaxShortPressDurationSeconds,
          maxPressIntervalDurationSeconds: properties.hornMaxPressIntervalDurationSeconds
      }),
      _hornButtonInterceptor(pins.hornButtonPin)
{
    // Setup timer
    Timer::setup();

    // Setup horn sounds
    HornSoundsLoader::setup(properties.hornSoundCollectionPath);

    // Setup horn button
    _hornButton = make_unique<Button<HornButtonHandler>>(
        &_hornButtonInterceptor,
        &_hornButtonHandler,
        vector<ButtonOptionType>{ButtonOptionType::ForceInitReleased}
    );
}
AudioScooterControlProgram::~AudioScooterControlProgram() {}

void AudioScooterControlProgram::_update() {
    _hornButton->update();
}
