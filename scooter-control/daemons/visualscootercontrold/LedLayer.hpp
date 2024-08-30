#ifndef LEDLAYER_H
#define LEDLAYER_H

#pragma once

// Standard includes
#include <stddef.h>

// External includes
#include "Color.hpp"

class LedLayer {
    public:
        LedLayer(size_t startLedIndex, size_t endLedIndex, size_t channelIndex);

        void enable();
        void disable();
        void setState(bool state);
        bool isEnabled();

        bool hasChanged();
        void aknowledgeChanges();

        Color getLedColor(size_t ledIndex);

        size_t getStartLedIndex();
        size_t getEndLedIndex();
        size_t getChannelIndex();
    protected:
        virtual Color _getLedColor(size_t ledIndex) = 0;

        virtual void _onEnable();
        virtual void _onDisable();

        bool _state = false;

        bool _hasChanged = false;

        size_t _startLedIndex;
        size_t _endLedIndex;
        size_t _channelIndex;
};

#endif