#include "LedLayer.hpp"

LedLayer::LedLayer(size_t startLedIndex, size_t endLedIndex, size_t channelIndex) {
    _startLedIndex = startLedIndex;
    _endLedIndex = endLedIndex;
    _channelIndex = channelIndex;
}

void LedLayer::enable() {
    if (!_state) {
        _state = true;
        _onEnable();
        _hasChanged = true;
    }
}
void LedLayer::disable() {
    if (_state) {
        _state = false;
        _onDisable();
        _hasChanged = true;
    }
}
void LedLayer::setState(bool state) {
    if (state) {
        enable();
    } else {
        disable();
    }
}
bool LedLayer::isEnabled() {
    return _state;
}

bool LedLayer::hasChanged() {
    return _hasChanged;
}
void LedLayer::aknowledgeChanges() {
    _hasChanged = false;
}

Color LedLayer::getLedColor(size_t ledIndex) {
    if (ledIndex < _startLedIndex || ledIndex > _endLedIndex) {
        cerr << "SolidColorLedLayer::_getLedColor: ledIndex " << ledIndex << " is out of range [" << _startLedIndex << ", " << _endLedIndex << "]" << endl;

        return Color::BLACK;
    }

    return _getLedColor(ledIndex);
}

size_t LedLayer::getStartLedIndex() {
    return _startLedIndex;
}
size_t LedLayer::getEndLedIndex() {
    return _endLedIndex;
}
size_t LedLayer::getChannelIndex() {
    return _channelIndex;
}

void LedLayer::_onEnable() {}
void LedLayer::_onDisable() {}