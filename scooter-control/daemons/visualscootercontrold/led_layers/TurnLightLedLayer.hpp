#ifndef TURNLIGHTLEDLAYER_H
#define TURNLIGHTLEDLAYER_H

#pragma once

// Standard includes
#include <vector>
using std::vector;

// Scooter control library includes
#include "timers/ElapsedTimeUpdatable.hpp"

// External includes
#include "../LedLayer.hpp"
#include "../Color.hpp"

class TurnLightLedLayer : public LedLayer, public ElapsedTimeUpdatable {
    public:
        struct AnimationProperties {
            bool timeDisplacementEndOnOne = true;
            bool oppositeDirection = false;
            double duration;
            double blackDurationPercentage = 50.0;
            double easingInDurationPercentage = 25.0;
        };
        struct Properties {
            Color color;
            AnimationProperties animationProperties;
        };
        TurnLightLedLayer(size_t startLedIndex, size_t endLedIndex, size_t channelIndex, Properties properties);

        void setColor(Color color);
        Color getColor();
    protected:
        Color _getLedColor(size_t ledIndex) override;

        void _onEnable() override;
        void update(double elapsedTimeSeconds) override;
    private:
        double _getEasedInTime(double time);
        double _getEasedOutTime(double time);

        Color _color;

        size_t _ledCount;
        vector<double> _ledSectionTimeDisplacements;

        bool _animationOppositeDirection;

        double _animationDurationSeconds;

        double _animationEasingInDurationPercentage;
        double _animationEasingOutDurationPercentage;
        double _animationBlackDurationPercentage;

        double _animationEasingInAndOutDurationPercentage;

        double _animationTime = 0;
        double _animationProgress = 0;
};

#endif