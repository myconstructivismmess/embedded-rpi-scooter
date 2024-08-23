# Add the wiringPi library
find_library(WIRINGPI_LIB wiringPi REQUIRED)

# Add the scootercontrol library
add_library(scootercontrol STATIC
    button/Button.hpp
    button/ButtonOptionType.h
    button/handlers/ButtonHandlerBase.hpp
    button/handlers/DefaultButtonHandler.hpp
    button/handlers/LatchButtonHandler.hpp
    button/interceptors/ButtonInterceptorBase.hpp
    button/interceptors/GPIOButtonInterceptor.hpp

    timers/Timer.h
    timers/Timer.cpp
    timers/IntervalTimer.h
    timers/IntervalTimer.cpp
    timers/ElapsedTimeUpdatable.h
    timers/ElapsedTimeUpdatable.cpp

    utils/Program.hpp
)

# Set the C++ standard to C++23
target_compile_features(scootercontrol PUBLIC cxx_std_23)

# Link the wiringPi library with scootercontrol
target_link_libraries(scootercontrol PUBLIC
    wiringPi
)

# Ensure the library's include directories are visible to targets that link to it
target_include_directories(scootercontrol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})