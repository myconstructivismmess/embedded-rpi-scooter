// Class Definition
#include "ElapsedTimeUpdatable.hpp"

ElapsedTimeUpdatable::ElapsedTimeUpdatable() {
    Timer::_registerElapsedTimeUpdatable(this);
}
ElapsedTimeUpdatable::~ElapsedTimeUpdatable() {
    Timer::_unregisterElapsedTimeUpdatable(this);
}
