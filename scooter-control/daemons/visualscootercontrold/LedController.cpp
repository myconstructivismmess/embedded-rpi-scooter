#include "LedController.hpp"

// Standard includes
#include <exception>
using std::invalid_argument;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <algorithm>
using std::find;

ws2811_t* LedController::_ledStripsPtr = nullptr;
vector<vector<shared_ptr<LedLayer>>> LedController::_ledStripsLayers = vector<vector<shared_ptr<LedLayer>>>();
vector<size_t> LedController::_ledStripsLedCount = vector<size_t>();
const int LedController::VALID_GPIO_PIN_NUMBERS[] = { 12, 18, 21 };

void LedController::setup(ws2811_t *ledStripsPtr, vector<size_t> ledStripsLedCount, vector<shared_ptr<LedLayer>> layers) {
    _ledStripsPtr = ledStripsPtr;
    _ledStripsLedCount = ledStripsLedCount;

    // Initialize led strips
    if (ws2811_init(ledStripsPtr) != WS2811_SUCCESS) {
        throw invalid_argument("LedController::setup: Failed to initialize led strips.");
    }

    // Setup layers by led strip index
    _ledStripsLayers = vector<vector<shared_ptr<LedLayer>>>();

    for (unsigned int i = 0; i < ledStripsLedCount.size(); i++) {
        _ledStripsLayers.push_back(vector<shared_ptr<LedLayer>>());
    }

    for (unsigned int i = 0; i < layers.size(); i++) {
        shared_ptr<LedLayer> layer = layers[i];
        _ledStripsLayers[layer->getChannelIndex()].push_back(layer);
    }
}
void LedController::clean() {
    ws2811_fini(_ledStripsPtr);
}

void LedController::update() {
    // TODO: Maybe implement a way to update only the led that have changed
    bool stripsUpdated = false;
    for (unsigned int i = 0; i < _ledStripsLayers.size(); i++) {
        bool stripUpdated = false;
        
        for (unsigned int j = 0; j < _ledStripsLayers.size(); j++) {
            shared_ptr<LedLayer> layer = _ledStripsLayers[i][j];

            if (layer->hasChanged()) {
                stripUpdated = true;
                stripsUpdated = true;
                layer->aknowledgeChanges();
                break;
            }
        }

        if (stripUpdated) {
            _renderLedStrips(i);
        }
    }

    if (stripsUpdated) {
        _pushLedStripsChanges();
    }
}

void LedController::_setLedColor(size_t ledStripIndex, size_t ledIndex, Color color) {
    if (ledStripIndex < 0 || ledStripIndex >= _ledStripsLedCount.size()) {
        cerr << "LedController::_setLedColor: Invalid led strip index provided." << endl;
        return;
    }

    if (ledIndex < 0 || ledIndex >= _ledStripsLedCount[ledStripIndex]) {
        cerr << "LedController::_setLedColor: Invalid led index provided." << endl;
        return;
    }

    _ledStripsPtr->channel[ledStripIndex].leds[ledIndex] = color.toHex();
}

void LedController::_renderLedStrips(size_t ledStripIndex) {
    if (ledStripIndex < 0) {
        cerr << "LedController::_renderLedStrips: Invalid led strip index provided." << endl;
        return;
    }

    if (_ledStripsLayers.size() <= ledStripIndex) {
        cerr << "LedController::_renderLedStrips: Invalid led strip index provided " << ledStripIndex << "." << endl;
        return;
    }

    set<size_t> renderedLedIndexes = set<size_t>();
    for (size_t i = 0; i < _ledStripsLayers[ledStripIndex].size(); i++) {
        shared_ptr<LedLayer> layer = _ledStripsLayers[ledStripIndex][i];

        if (!layer->isEnabled()) {
            continue;
        }

        for (size_t j = layer->getStartLedIndex(); j <= layer->getEndLedIndex(); j++) {
            if (renderedLedIndexes.contains(j)) {
                continue;
            }

            _setLedColor(ledStripIndex, j, layer->getLedColor(j));

            renderedLedIndexes.insert(j);
        }
    }
}

bool LedController::_pushLedStripsChanges() {
    if (ws2811_render(_ledStripsPtr) != WS2811_SUCCESS) {
        cerr << "LedController::_renderLedStrips: Failed to render led strips." << endl;
        return false;
    }

    return true;
}
