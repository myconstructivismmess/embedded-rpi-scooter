#ifndef INTERVALTIMER_H
#define INTERVALTIMER_H

#pragma once

// External includes
#include "ElapsedTimeUpdatable.hpp"

/**
 * This class provides functionality to track elapsed time, reset the elapsed time, update the elapsed time,
 * and consume elapsed time in discrete blocks. The concept of consuming blocks of time is particularly
 * useful in scenarios where tasks or events need to be triggered at regular intervals.
*/
class IntervalTimer : public ElapsedTimeUpdatable {
    public:
        IntervalTimer();

        /**
         * @brief Reset the elapsed time to zero.
         */
        void reset();

        /**
         * @brief Consume available time blocks and return the count of full blocks consumed.
         * 
         * @param timeBlockDurationSeconds The duration of each time block in seconds.
         * @param maxBlocksConsumed The maximum number of blocks to consume. If -1, there is no limit.
         * @return The number of complete blocks of time that have been consumed.
        */
        int consumeAvailableTimeBlocks(double timeBlockDurationSeconds, int maxBlocksConsumed = -1);
    protected:
        /**
         * @brief Update the elapsed time.
         * (This method is meant to be called by the Timer class)
         * 
         * @param elapsedTimeSeconds The amount of time to add to the elapsed time.
        */
        void update(double elapsedTimeSeconds) override;
    private:
        double _elapsedTimeSeconds;
};

#endif
