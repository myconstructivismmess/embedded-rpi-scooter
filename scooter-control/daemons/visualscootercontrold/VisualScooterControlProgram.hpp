#ifndef VISUALSCOOTERCONTROLPROGRAM_H
#define VISUALSCOOTERCONTROLPROGRAM_H

#pragma once

// Standard includes
#include <memory>
using std::unique_ptr;
using std::shared_ptr;

#include <vector>
using std::vector;

// Scooter control library includes
#include "utils/Program.hpp"
#include "button/Button.hpp"
#include "button/interceptors/GPIOButtonInterceptor.hpp"
#include "button/handlers/DefaultButtonHandler.hpp"

// External libraries includes
#include <ws2811.h>

// External includes
#include "LedController.hpp"
#include "Color.hpp"
#include "led_layers/SolidColorLedLayer.hpp"
#include "led_layers/TurnLightLedLayer.hpp"

class VisualScooterControlProgram : public Program {
    public:
        struct Pins {
            int turnLeftSignalButtonPin;
            int turnRightSignalButtonPin;
            int lightOnOffButtonPin;
            int brakeDetectorPin;
            int frontLedsPin;
            int backLedsPin;
        };
        struct Properties {
            int turnSignalColor;
            ws2811_t *ledStripsPtr;
            vector<size_t> ledStripsLedCount;
        };
        VisualScooterControlProgram(Pins pins, Properties properties);
        ~VisualScooterControlProgram();

        static void _onLightOnOffStateChange(bool newState);
        static void _onTurnLeftStateChange(bool newState);
        static void _onTurnRightStateChange(bool newState);
        // No brake for now
        // static void _onBrakeStateChange(bool newState);
    protected:
        void _update() override;
    private:
        static VisualScooterControlProgram* _instance;

        DefaultButtonHandler _lightOnOffButtonHandler;
        GPIOButtonInterceptor _lightOnOffButtonInterceptor;
        unique_ptr<Button<DefaultButtonHandler>> _lightOnOffButton;

        DefaultButtonHandler _turnLeftSignalButtonHandler;
        GPIOButtonInterceptor _turnLeftSignalButtonInterceptor;
        unique_ptr<Button<DefaultButtonHandler>> _turnLeftSignalButton;

        DefaultButtonHandler _turnRightSignalButtonHandler;
        GPIOButtonInterceptor _turnRightSignalButtonInterceptor;
        unique_ptr<Button<DefaultButtonHandler>> _turnRightSignalButton;

        // No brake for now
        // DefaultButtonHandler _brakeDetectorHandler;
        // GPIOButtonInterceptor _brakeDetectorInterceptor;
        // unique_ptr<Button<DefaultButtonHandler>> _brakeDetector;

        shared_ptr<SolidColorLedLayer> _frontLightOnOffLedLayer;
        shared_ptr<TurnLightLedLayer> _frontTurnSignalLedLayer;
};

#endif
