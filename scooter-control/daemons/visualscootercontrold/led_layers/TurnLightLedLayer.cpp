#include "TurnLightLedLayer.hpp"

// Standard includes
#include <stdexcept>
using std::invalid_argument;

#include <cmath>
using std::pow;
using std::min;
using std::max;

TurnLightLedLayer::TurnLightLedLayer(size_t startLedIndex, size_t endLedIndex, size_t channelIndex, Properties properties)
    : LedLayer(startLedIndex, endLedIndex, channelIndex)
{
    _color = properties.color;

    bool timeDisplacementEndOnOne = properties.animationProperties.timeDisplacementEndOnOne;
    double timeDisplacementDivisor = (double)(_ledCount - timeDisplacementEndOnOne ? 1 : 0);
    _ledCount = endLedIndex - startLedIndex + 1;
    _ledSectionTimeDisplacements = vector<double>(_ledCount);
    for (size_t i = 0; i < _ledCount; i++) {
        _ledSectionTimeDisplacements[i] = (double)i / (double)(_ledCount - 1);
    }

    _animationOppositeDirection = properties.animationProperties.oppositeDirection;

    double animationDurationSeconds = properties.animationProperties.duration;
    double animationEasingInDurationPercentage = properties.animationProperties.easingInDurationPercentage / 100.0;
    double animationBlackDurationPercentage = properties.animationProperties.blackDurationPercentage / 100.0;

    if ((animationBlackDurationPercentage + animationEasingInDurationPercentage) >= 1) {
        throw invalid_argument("TurnLightLedLayer: blackDurationPercentage + easingInDurationPercentage must be less than 1");
    }

    _animationDurationSeconds = animationDurationSeconds;

    _animationEasingInDurationPercentage = animationEasingInDurationPercentage;
    _animationEasingOutDurationPercentage = 1.0 - animationEasingInDurationPercentage - animationBlackDurationPercentage;
    _animationBlackDurationPercentage = animationBlackDurationPercentage;

    _animationEasingInAndOutDurationPercentage = 1.0 - animationBlackDurationPercentage;
}

void TurnLightLedLayer::setColor(Color color) {
    _color = color;
    _hasChanged = true;
}
Color TurnLightLedLayer::getColor() {
    return _color;
}

Color TurnLightLedLayer::_getLedColor(size_t ledIndex) {
    size_t ledSectionIndex = ledIndex - _startLedIndex;

    if (_animationOppositeDirection) {
        ledSectionIndex = _ledCount - 1 - ledSectionIndex;
    }

    if (_animationProgress <= _animationEasingInDurationPercentage) {
        double animationEasingInProgress = _animationProgress / _animationEasingInDurationPercentage;
        double animationEasingInDisplacedProgress = _ledSectionTimeDisplacements[ledSectionIndex] - 1.0 + animationEasingInProgress * 2.0;
        double animationEasingInEasedTime = _getEasedInTime(animationEasingInDisplacedProgress);

        return Color::mix(
            Color::BLACK,
            _color,
            animationEasingInEasedTime
        );
    }

    if (_animationProgress <= _animationEasingInAndOutDurationPercentage) {
        double animationEasingOutProgress = (_animationProgress - _animationEasingInDurationPercentage) / _animationEasingOutDurationPercentage;
        double animationEasingOutDisplacedProgress = _ledSectionTimeDisplacements[ledSectionIndex] - 1.0 + animationEasingOutProgress * 2.0;
        double animationEasingOutEasedTime = _getEasedOutTime(animationEasingOutDisplacedProgress);

        return Color::mix(
            _color,
            Color::BLACK,
            animationEasingOutEasedTime
        );
    }

    return Color::BLACK;
}

void TurnLightLedLayer::_onEnable() {
    _animationTime = 0;
}

void TurnLightLedLayer::update(double elapsedTimeSeconds) {
    if (!_state) {
        return;
    }

    _animationTime += elapsedTimeSeconds;

    while (_animationTime >= _animationDurationSeconds) {
        _animationTime = _animationTime - _animationDurationSeconds;
    }

    _animationProgress = _animationTime / _animationDurationSeconds;

    _hasChanged = true;
}

double TurnLightLedLayer::_getEasedInTime(double time01) {
    time01 = max(0.0, min(time01, 1.0));
    return 1.0 - pow(1.0 - time01, 3.0);
}

double TurnLightLedLayer::_getEasedOutTime(double time01) {
    time01 = max(0.0, min(time01, 1.0));
    return 1.0 - pow(1.0 - time01, 3.0);
}
