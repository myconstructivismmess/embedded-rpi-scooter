// Class Definition
#include "Timer.hpp"

// Functions
/**
 * @brief Convert a time point to a double in seconds.
*/
double timePointToSeconds(const chrono::time_point<chrono::high_resolution_clock>& timePoint) {
    auto duration = timePoint.time_since_epoch();
    return chrono::duration<double>(duration).count();
}

void Timer::setup() {
    _updateTime();
}

void Timer::update() {
    // Calculate the elapsed time
    auto lastTimeSeconds = _sTime;

    _updateTime();

    auto elapsedTime = _sTime - lastTimeSeconds;
    double elapsedTimeSeconds = chrono::duration<double>(elapsedTime).count();

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

double Timer::getTimeSeconds() {
    return timePointToSeconds(_sTime);
}

void Timer::_updateTime() {
    _sTime = chrono::high_resolution_clock::now();
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

chrono::time_point<chrono::high_resolution_clock> Timer::_sTime = chrono::high_resolution_clock::now();
vector<ElapsedTimeUpdatable*> Timer::_sUpdatables = vector<ElapsedTimeUpdatable*>();
