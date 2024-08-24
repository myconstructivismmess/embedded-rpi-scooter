#ifndef AUDIOSCOOTERCONTROLPROGRAM_H
#define AUDIOSCOOTERCONTROLPROGRAM_H

#pragma once

// Standard includes
#include <string>
using std::string;

#include <memory>
using std::unique_ptr;
using std::make_unique;
using std::move;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <filesystem>
namespace fs = std::filesystem;
using fs::path;

#include <stdexcept>
using std::invalid_argument;
using std::exception;

#include <regex>
using std::regex;
using std::regex_match;

// Scooter control library includes
#include "utils/Program.hpp"
#include "button/Button.hpp"
#include "button/interceptors/GPIOButtonInterceptor.hpp"
#include "button/ButtonOptionType.h"
#include "timers/Timer.hpp"

// External includes
#include "HornButtonHandler.hpp"
#include "HornSoundPlayer.hpp"
#include "HornSoundsLoader.hpp"

class AudioScooterControlProgram : public Program {
    public:
        struct AudioScooterControlProgramPins {
            int hornButtonPin;
        };
        struct AudioScooterControlProgramData {
            string hornSoundCollectionPath;
            float hornMaxShortPressDurationSeconds;
            float hornMaxPressIntervalDurationSeconds;
        };
        AudioScooterControlProgram(AudioScooterControlProgramPins pins, AudioScooterControlProgramData data)
            : Program(),
                _hornButtonHandler(HornButtonHandlerProperties {
                    maxShortPressDurationSeconds: data.hornMaxShortPressDurationSeconds,
                    maxPressIntervalDurationSeconds: data.hornMaxPressIntervalDurationSeconds
                }),
                _hornButtonInterceptor(pins.hornButtonPin)
        {
            // Setup timer
            Timer::setup();

            // Setup horn sounds
            HornSoundsLoader::setup(data.hornSoundCollectionPath);

            // Setup horn button
            _hornButton = make_unique<Button<HornButtonHandler>>(
                &_hornButtonInterceptor,
                &_hornButtonHandler,
                vector<ButtonOptionType>{ButtonOptionType::ForceInitReleased}
            );
        }
        ~AudioScooterControlProgram() {

        }
    protected:
        void _update() override {
            _hornButton->update();
        }
    private:
        HornButtonHandler _hornButtonHandler;
        GPIOButtonInterceptor _hornButtonInterceptor;
        unique_ptr<Button<HornButtonHandler>> _hornButton;
};

#endif
