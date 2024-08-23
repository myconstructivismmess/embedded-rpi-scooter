#ifndef DEFAULTBUTTONHANDLER_H
#define DEFAULTBUTTONHANDLER_H

// External includes
#include "ButtonHandlerBase.hpp"

class DefaultButtonHandler : public ButtonHandlerBase {
    public:
        DefaultButtonHandler(void(*onValueChangeCallback)(bool))
            : _onValueChangeCallback(onValueChangeCallback) {}
    protected:
        void _onEnabled() override {
            if (_realValue != _value) {
                _value = _realValue;
                _onValueChangeCallback(_value);
            }
        };
        void _onDisabled() override {
            if (_value) {
                _value = false;
                _onValueChangeCallback(_value);
            }
        }

        void _onValueRaised() override {
            _realValue = true;

            if (isEnabled()) {
                _value = true;
                _onValueChangeCallback(_value);
            }
        }
        void _onValueLowered() override {
            _realValue = false;

            if (isEnabled()) {
                _value = false;
                _onValueChangeCallback(_value);
            }
        }
    private:
        void(*_onValueChangeCallback)(bool);
        
        bool _realValue;
        bool _value;
};

#endif
