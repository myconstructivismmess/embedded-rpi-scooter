// Class Definition
#include "LatchButtonHandler.hpp"

LatchButtonHandler::LatchButtonHandler(void(*onValueChangeCallback)(bool), bool startValue, bool switchOnLowered)
    : _onValueChangeCallback(onValueChangeCallback),
      _switchOnLowered(switchOnLowered),
      _value(startValue) {}

bool LatchButtonHandler::getValue() {
    return _value;
}
void LatchButtonHandler::setValue(bool value, bool triggerCallback) {
    _value = value;

    if (triggerCallback) {
        _onValueChangeCallback(_value);
    }
}

void LatchButtonHandler::_onValueRaised() {
    if (isEnabled() && !_switchOnLowered) {
        setValue(!_value);
    }
}
void LatchButtonHandler::_onValueLowered() {
    if (isEnabled() && _switchOnLowered) {
        setValue(!_value);
    }
}
