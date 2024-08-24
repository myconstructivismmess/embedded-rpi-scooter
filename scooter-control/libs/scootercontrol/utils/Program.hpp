#ifndef PROGRAM_H
#define PROGRAM_H

#pragma once

// Standard includes
#include <csignal>

#include <iostream>
using std::cout;
using std::endl;

#include <stdexcept>
using std::runtime_error;

// External includes
#include "../timers/Timer.hpp"

/**
 * @brief Program class use to abstract the main loop of the program.
 * 
 * This class provides functionality to run the main loop of the program and update the program at regular intervals as wall as updating the Timer.
 * Only one instance of this class should be created and run at a time.
*/
class Program {
    public:
        Program();
        ~Program();

        /**
         * @brief Run one iteration of the program.
        */
        void update();

        /**
         * @brief Run the program until the program is stopped.
        */
        void runUntilStopped();

        /**
         * @brief Exit the program.
        */
        void exit();

        /**
         * @brief Reset the shouldExit flag to false.
        */
        void resetExit();
        
        /**
         * @brief Register signal handlers for SIGINT, SIGTERM, and SIGKILL.
        */
        void registerSignalHandlers();
        /**
         * @brief Unregister signal handlers for SIGINT, SIGTERM, and SIGKILL.
        */
        void unregisterSignalHandlers();
    protected:
        virtual void _update() = 0;
    private:
        /**
         * @brief Signal handler for SIGINT, SIGTERM, and SIGKILL.
        */
        static void _onSignal(int sigNo);

        static Program* _instance;

        bool _shouldExit = false;
};

#endif
