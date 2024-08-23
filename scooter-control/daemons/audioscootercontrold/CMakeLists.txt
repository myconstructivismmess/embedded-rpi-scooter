# Add audioscootercontrold executable
add_executable(audioscootercontrold
    main.cpp
    AudioScooterControlProgram.hpp
    HornButtonHandler.hpp
    HornSoundPlayer.hpp
    HornSoundsLoader.hpp
    Audio.hpp
)

# Set the C++ standard to C++23
target_compile_features(audioscootercontrold PUBLIC cxx_std_23)

# Link the scootercontrol library to the audioscootercontrold executable
target_link_libraries(audioscootercontrold PUBLIC
    scootercontrol
)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

# Link SFML audio library to the audioscootercontrold executable
target_link_libraries(audioscootercontrold PUBLIC
    sfml-audio
    sfml-system
)
