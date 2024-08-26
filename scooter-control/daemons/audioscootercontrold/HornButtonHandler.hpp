#ifndef HORNBUTTONHANDLER_H
#define HORNBUTTONHANDLER_H

#pragma once

// Scooter control library includes
#include "button/handlers/ButtonHandlerBase.hpp"
#include "button/Button.hpp"

class HornButtonHandler : public ButtonHandlerBase {
    template<class T>
    friend class Button;

    public:
        struct Properties {
            float maxShortPressDurationSeconds;
            float maxPressIntervalDurationSeconds;
        };
        HornButtonHandler(Properties properties);
    protected:
        void _onDisabled() override;

        void _onValueRaised() override;
        void _onValueLowered() override;
        
        void _onUpdate() override;
    private:
        void _onValueChange(bool value);

        float _maxShortPressDurationSeconds, _maxPressIntervalDurationSeconds;

        bool _value;
        unsigned int _pressCount;
        bool _isRepeatableSoundInitiated;
        double _lastPressTimeSeconds;
        double _lastReleaseTimeSeconds;
};

#endif
