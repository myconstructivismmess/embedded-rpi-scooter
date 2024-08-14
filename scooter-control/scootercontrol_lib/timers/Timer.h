#ifndef TIMER_H
#define TIMER_H

#pragma once

// Standard includes
#include <vector>
using std::vector;

// Forward declaration of ElapsedTimeUpdatable
class ElapsedTimeUpdatable;

// External includes
#include "ElapsedTimeUpdatable.h"

class Timer
{
friend class ElapsedTimeUpdatable;
public:
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

    static double _sTimeSeconds;
    static vector<ElapsedTimeUpdatable*> _sUpdatables;
};

#endif
