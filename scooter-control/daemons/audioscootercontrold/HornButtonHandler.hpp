#ifndef HORNBUTTONHANDLER_H
#define HORNBUTTONHANDLER_H

#pragma once

// Standard includes
#include <vector>
using std::vector;

#include <stdexcept>
using std::invalid_argument;

#include <string>
using std::to_string;

// Scooter control library includes
#include "button/handlers/ButtonHandlerBase.hpp"
#include "button/Button.hpp"
#include "timers/Timer.hpp"

// External includes
#include "HornSoundPlayer.hpp"

struct HornButtonHandlerProperties {
    float maxShortPressDurationSeconds;
    float maxPressIntervalDurationSeconds;
};

class HornButtonHandler : public ButtonHandlerBase {
    template<class T>
    friend class Button;

    public:
        HornButtonHandler(HornButtonHandlerProperties properties)
            : ButtonHandlerBase(),
            _maxShortPressDurationSeconds(properties.maxShortPressDurationSeconds),
            _maxPressIntervalDurationSeconds(properties.maxPressIntervalDurationSeconds)
        {
            _value = false;
            _pressCount = 0;
            _isRepeatableSoundInitiated = false;
        }
    protected:
        void _onDisabled() override {
            _onValueChange(false);
        }

        void _onValueRaised() override {
            _onValueChange(true);
        }
        void _onValueLowered() override {
            _onValueChange(false);
        }
        
        void _onUpdate() override {
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
    private:
        void _onValueChange(bool value) {
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

        float _maxShortPressDurationSeconds, _maxPressIntervalDurationSeconds;

        bool _value;
        unsigned int _pressCount;
        bool _isRepeatableSoundInitiated;
        double _lastPressTimeSeconds;
        double _lastReleaseTimeSeconds;
};

#endif
