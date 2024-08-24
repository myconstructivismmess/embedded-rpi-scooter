// Class definition
#include "ButtonHandlerBase.hpp"

void ButtonHandlerBase::enable() {
    if (!_enabled) {
        _enabled = true;
        _onEnabled();
    }
}
void ButtonHandlerBase::disable() {
    if (_enabled) {
        _enabled = false;
        _onDisabled();
    }
}
void ButtonHandlerBase::setState(bool enabled) {
    if (enabled) {
        enable();
    } else {
        disable();
    }
}
bool ButtonHandlerBase::isEnabled() {
    return _enabled;
}
