#ifndef ELAPSEDTIMEUPDATABLE_H
#define ELAPSEDTIMEUPDATABLE_H

#pragma once

// External includes
#include "Timer.hpp"

/**
 * @brief Interface for classes that can be updated with elapsed time.
 */
class ElapsedTimeUpdatable {
    friend class Timer;

    public:
        ElapsedTimeUpdatable();
        ~ElapsedTimeUpdatable();
    protected:
        /**
         * @brief Update the elapsed time.
         * (This method is meant to be called by the Timer class)
         * 
         * @param elapsedTimeSeconds The amount of time to add to the elapsed time.
         */
        virtual void update(double elapsedTimeSeconds) = 0;
};

#endif
