#ifndef AUDIOSCOOTERCONTROLPROGRAM_H
#define AUDIOSCOOTERCONTROLPROGRAM_H

#pragma once

// Standard includes
#include <memory>
using std::unique_ptr;

#include <string>
using std::string;

// Scooter control library includes
#include "utils/Program.hpp"
#include "button/Button.hpp"
#include "button/interceptors/GPIOButtonInterceptor.hpp"

// External includes
#include "HornButtonHandler.hpp"

class AudioScooterControlProgram : public Program {
    public:
        struct Pins {
            int hornButtonPin;
        };
        struct Properties {
            string hornSoundCollectionPath;
            float hornMaxShortPressDurationSeconds;
            float hornMaxPressIntervalDurationSeconds;
        };
        AudioScooterControlProgram(Pins pins, Properties properties);
        ~AudioScooterControlProgram();
    protected:
        void _update() override;
    private:
        HornButtonHandler _hornButtonHandler;
        GPIOButtonInterceptor _hornButtonInterceptor;
        unique_ptr<Button<HornButtonHandler>> _hornButton;
};

#endif
