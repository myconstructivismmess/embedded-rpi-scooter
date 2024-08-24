# Add the wiringPi library
find_library(WIRINGPI_LIB wiringPi REQUIRED)

# Add the scootercontrol library
add_library(scootercontrol STATIC
    button/Button.hpp
    
    button/ButtonOptionType.h

    button/handlers/ButtonHandlerBase.hpp
    button/handlers/ButtonHandlerBase.cpp

    button/handlers/DefaultButtonHandler.hpp
    button/handlers/DefaultButtonHandler.cpp

    button/handlers/LatchButtonHandler.hpp
    button/handlers/LatchButtonHandler.cpp


    button/interceptors/ButtonInterceptorBase.hpp
    button/interceptors/ButtonInterceptorBase.cpp

    button/interceptors/GPIOButtonInterceptor.hpp
    button/interceptors/GPIOButtonInterceptor.cpp


    timers/Timer.hpp
    timers/Timer.cpp

    timers/IntervalTimer.hpp
    timers/IntervalTimer.cpp

    timers/ElapsedTimeUpdatable.hpp
    timers/ElapsedTimeUpdatable.cpp


    utils/Program.hpp
    utils/Program.cpp
)

# Link the wiringPi library with scootercontrol
target_link_libraries(scootercontrol PUBLIC
    wiringPi
)

# Ensure the library's include directories are visible to targets that link to it
target_include_directories(scootercontrol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})