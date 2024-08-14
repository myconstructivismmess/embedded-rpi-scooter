// Class include
#include "Timer.h"

// Standard includes
#include <time.h>
#include <iostream>
using std::cerr;
using std::endl;

vector<ElapsedTimeUpdatable*> Timer::_sUpdatables;

void Timer::update() {
    double lastTimeSeconds = _sTimeSeconds;

    _updateTime();

    double elapsedTimeSeconds = _sTimeSeconds - lastTimeSeconds;

    // Simple check to make sure time is moving forward
    if (elapsedTimeSeconds < 0.0) {
        cerr << "Timer: Time went backwards, probably due to system time change. Ignoring update." << endl; 
        return;
    }

    // Update all updatables
    for (vector<ElapsedTimeUpdatable*>::iterator it = _sUpdatables.begin(); it != _sUpdatables.end(); ++it) {
        (*it)->update(elapsedTimeSeconds);
    }
}

double Timer::getTimeSeconds()
{
    return _sTimeSeconds;
}

void Timer::_updateTime() {
    time_t current_time = time(nullptr);
    _sTimeSeconds = (double)current_time;
}

void Timer::_registerElapsedTimeUpdatable(ElapsedTimeUpdatable* updatable) {
    _sUpdatables.push_back(updatable);
}
void Timer::_unregisterElapsedTimeUpdatable(ElapsedTimeUpdatable* updatable) {
    for (vector<ElapsedTimeUpdatable*>::iterator it = _sUpdatables.begin(); it != _sUpdatables.end(); ++it) {
        if (*it == updatable) {
            _sUpdatables.erase(it);
            return;
        }
    }
}
