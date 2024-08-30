#ifndef SOLIDCOLORLEDLAYER_H
#define SOLIDCOLORLEDLAYER_H

#pragma once

// External includes
#include "../LedLayer.hpp"
#include "../Color.hpp"

class SolidColorLedLayer : public LedLayer {
    public:
        SolidColorLedLayer(size_t startLedIndex, size_t endLedIndex, size_t channelIndex, Color color);

        void setColor(Color color);
        Color getColor();
    protected:
        Color _getLedColor(size_t ledIndex) override;
    private:
        Color _color;
};

#endif