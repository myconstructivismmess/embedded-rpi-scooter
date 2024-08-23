#ifndef PROGRAM_H
#define PROGRAM_H

#pragma once

// Standard includes
#include <csignal>
#include <iostream>
using std::cout;
using std::endl;

// External includes
#include "../timers/Timer.h"

class Program {
    public:
        /**
         * @brief Program class use to abstract the main loop of the program.
        */
        Program() {
            _shouldExit = false;
        }

        void update() {
            Timer::update();
            _update();
        }

        void runUntilStopped() {
            while (!_shouldExit) {
                update();
            }
        }

        static void exit() {
            _shouldExit = true;
        }
        
        static void registerSignalHandlers() {
            signal(SIGINT, _onSignal);
            signal(SIGTERM, _onSignal);
            signal(SIGKILL, _onSignal);
        }
    protected:
        virtual void _update() = 0;
    private:
        static void _onSignal(int sigNo) {
            cout << endl << "Received signal " << sigNo << " Exiting..." << endl;

            exit();
        }

        static bool _shouldExit;
};

bool Program::_shouldExit = false;

#endif
