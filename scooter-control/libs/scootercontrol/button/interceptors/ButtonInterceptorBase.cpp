// Class Definition
#include "ButtonInterceptorBase.hpp"

ButtonInterceptorBase::ButtonInterceptorBase(bool reverseSignal)
    : _reverseSignal(reverseSignal) {}

bool ButtonInterceptorBase::isPressed() {
    return _reverseSignal
        ? !_isPressed()
        : _isPressed();
}
