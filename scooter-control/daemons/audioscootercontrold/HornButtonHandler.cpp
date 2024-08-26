#include "HornButtonHandler.hpp"

// Scooter control library includes
#include "timers/Timer.hpp"

// External includes
#include "HornSoundPlayer.hpp"

HornButtonHandler::HornButtonHandler(Properties properties)
    : ButtonHandlerBase(),
      _maxShortPressDurationSeconds(properties.maxShortPressDurationSeconds),
      _maxPressIntervalDurationSeconds(properties.maxPressIntervalDurationSeconds)
{
    _value = false;
    _pressCount = 0;
    _isRepeatableSoundInitiated = false;
}

void HornButtonHandler::_onDisabled() {
    _onValueChange(false);
}

void HornButtonHandler::_onValueRaised() {
    _onValueChange(true);
}
void HornButtonHandler::_onValueLowered() {
    _onValueChange(false);
}
        
void HornButtonHandler::_onUpdate() {
    if (_pressCount <= 0) {
        return;
    }

    double timeSeconds = Timer::getTimeSeconds();

    if (_value) {
        double timeSincePressInitiatedSeconds = timeSeconds - _lastPressTimeSeconds;

        if (timeSincePressInitiatedSeconds < _maxShortPressDurationSeconds) {
            return;
        }

        bool isLongSoundRepeatable = HornSoundPlayer::isLongHornSoundRepeatable(_pressCount);

        _isRepeatableSoundInitiated = isLongSoundRepeatable;
        HornSoundPlayer::playHornSound(_pressCount, true);

        _pressCount = 0;
    } else {
        double timeSinceReleaseInitiatedSeconds = timeSeconds - _lastReleaseTimeSeconds;

        if (timeSinceReleaseInitiatedSeconds < _maxPressIntervalDurationSeconds) {
            return;
        }

        HornSoundPlayer::playHornSound(_pressCount, false);

        _pressCount = 0;
    }
}

void HornButtonHandler::_onValueChange(bool value) {
    if (value) {
        if (!isEnabled()) {
            return;
        }
    } else {
        if (!_value) {
            return;
        }
    }

    _value = value;

    double timeSeconds = Timer::getTimeSeconds();

    if (value) {
        _lastPressTimeSeconds = timeSeconds;

        _pressCount++;

        if (!HornSoundPlayer::isLastHornSoundLong() && _pressCount >= HornSoundPlayer::getLastHornPressCount()) {
            HornSoundPlayer::playHornSound(_pressCount, false);

            _pressCount = 0;
        }
    } else {
        _lastReleaseTimeSeconds = timeSeconds;

        if (_isRepeatableSoundInitiated) {
            HornSoundPlayer::stopHornSound();

            _isRepeatableSoundInitiated = false;
        }
    }
}
