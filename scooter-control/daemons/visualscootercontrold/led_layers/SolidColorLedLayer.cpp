#include "SolidColorLedLayer.hpp"

SolidColorLedLayer::SolidColorLedLayer(size_t startLedIndex, size_t endLedIndex, size_t channelIndex, Color color)
    : LedLayer(startLedIndex, endLedIndex, channelIndex)
{
    _color = color;
}

void SolidColorLedLayer::setColor(Color color) {
    _color = color;
    _hasChanged = true;
}
Color SolidColorLedLayer::getColor() {
    return _color;
}

Color SolidColorLedLayer::_getLedColor(size_t ledIndex) {
    return _color;
}
