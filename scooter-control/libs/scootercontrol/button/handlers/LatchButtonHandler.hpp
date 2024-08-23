#ifndef LATCHBUTTONHANDLER_H
#define LATCHBUTTONHANDLER_H

// External includes
#include "ButtonHandlerBase.hpp"

class LatchButtonHandler : public ButtonHandlerBase {
    public:
        LatchButtonHandler(void(*onValueChangeCallback)(bool), bool startValue = false, bool switchOnLowered = false)
            : _onValueChangeCallback(onValueChangeCallback),
              _switchOnLowered(switchOnLowered),
              _value(startValue) {}

        bool getValue() {
            return _value;
        }
        void setValue(bool value, bool triggerCallback = true) {
            _value = value;

            if (triggerCallback) {
                _onValueChangeCallback(_value);
            }
        }
    protected:
        void _onValueRaised() override {
            if (isEnabled() && !_switchOnLowered) {
                setValue(!_value);
            }
        }
        void _onValueLowered() override {
            if (isEnabled() && _switchOnLowered) {
                setValue(!_value);
            }
        }
    private:
        void(*_onValueChangeCallback)(bool);
        const bool _switchOnLowered;

        bool _value;
};

#endif
