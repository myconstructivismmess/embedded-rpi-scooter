// Class Definition
#include "IntervalTimer.hpp"

IntervalTimer::IntervalTimer()
    : ElapsedTimeUpdatable()
{
    reset();
}

void IntervalTimer::reset() {
    _elapsedTimeSeconds = 0;
}

int IntervalTimer::consumeAvailableTimeBlocks(double timeBlockDurationSeconds, int maxBlocksConsumed) {
    int count = _elapsedTimeSeconds / timeBlockDurationSeconds;
    if (maxBlocksConsumed != -1 && count > maxBlocksConsumed)
    {
        count = maxBlocksConsumed;
    }

    _elapsedTimeSeconds -= (count * timeBlockDurationSeconds);

    return count;
}

void IntervalTimer::update(double elapsedTimeSeconds) {
    _elapsedTimeSeconds += elapsedTimeSeconds;
}
