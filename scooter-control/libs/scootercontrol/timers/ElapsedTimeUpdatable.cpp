// Class include
#include "ElapsedTimeUpdatable.h"

ElapsedTimeUpdatable::ElapsedTimeUpdatable()
{
    Timer::_registerElapsedTimeUpdatable(this);
}
ElapsedTimeUpdatable::~ElapsedTimeUpdatable()
{
    Timer::_unregisterElapsedTimeUpdatable(this);
}
