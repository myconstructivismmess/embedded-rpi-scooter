#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#pragma once

// Standard includes
#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;

// External libraries includes
#include <ws2811.h>

// External includes
#include "LedLayer.hpp"

class LedController {
    friend class LedLayer;
    public:
        static void setup(ws2811_t *ledStripsPtr, vector<size_t> ledStripsLedCount, vector<shared_ptr<LedLayer>> layers);
        static void clean();

        static void update();
    private:
        static void _setLedColor(size_t ledStripIndex, size_t ledIndex, Color color); 
        static void _renderLedStrips(size_t ledStripIndex);
        static bool _pushLedStripsChanges();

        static ws2811_t *_ledStripsPtr;

        static vector<vector<shared_ptr<LedLayer>>> _ledStripsLayers;
        static vector<size_t> _ledStripsLedCount;

        static const int VALID_GPIO_PIN_NUMBERS[];
};

#endif