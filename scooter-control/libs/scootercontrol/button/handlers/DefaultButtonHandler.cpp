// Class Definition
#include "DefaultButtonHandler.hpp"

DefaultButtonHandler::DefaultButtonHandler(void(*onValueChangeCallback)(bool))
    : _onValueChangeCallback(onValueChangeCallback) {}

void DefaultButtonHandler::_onEnabled() {
    if (_realValue != _value) {
        _value = _realValue;
        _onValueChangeCallback(_value);
    }
};
void DefaultButtonHandler::_onDisabled() {
    if (_value) {
        _value = false;
        _onValueChangeCallback(_value);
    }
}

void DefaultButtonHandler::_onValueRaised() {
    _realValue = true;

    if (isEnabled()) {
        _value = true;
        _onValueChangeCallback(_value);
    }
}
void DefaultButtonHandler::_onValueLowered() {
    _realValue = false;

    if (isEnabled()) {
        _value = false;
        _onValueChangeCallback(_value);
    }
}
