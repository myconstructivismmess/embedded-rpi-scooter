#ifndef TIMER_H
#define TIMER_H

#pragma once

// Standard includes
#include <vector>
using std::vector;

#include <chrono>
namespace chrono = std::chrono;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

// Forward declaration of ElapsedTimeUpdatable
class ElapsedTimeUpdatable;

// External includes
#include "ElapsedTimeUpdatable.hpp"

/**
 * @brief Timer class used to track time and update registered updatables.
*/
class Timer {
    friend class ElapsedTimeUpdatable;
    
    public:
        /**
         * @brief Setup the timer.
        */
        static void setup();

        /**
         * @brief Update the timer and all registered updatables.
        */
        static void update();

        /**
         * @brief Get the time of the last timer update in seconds.
         * 
         * @return The time of the last timer update in seconds.
        */
        static double getTimeSeconds();
    private:
        /**
         * @brief Update the time.
        */
        static void _updateTime();

        /**
         * @brief Register an ElapsedTimeUpdatable object.
         * 
         * @param updatable The object to register
        */
        static void _registerElapsedTimeUpdatable(ElapsedTimeUpdatable* updatable);
        /**
         * @brief Unregister an ElapsedTimeUpdatable object.
         * 
         * @param updatable The object to unregister
        */
        static void _unregisterElapsedTimeUpdatable(ElapsedTimeUpdatable* updatable);

        static chrono::time_point<chrono::high_resolution_clock> _sTime;
        static vector<ElapsedTimeUpdatable*> _sUpdatables;
};

#endif
