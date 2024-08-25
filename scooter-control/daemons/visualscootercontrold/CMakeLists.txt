# Add audioscootercontrold executable
add_executable(visualscootercontrold
    main.cpp

    VisualScooterControlProgram.h
    VisualScooterControlProgram.cpp

    Color.hpp
    Color.cpp
)

# Set the C++ standard to C++23
target_compile_features(visualscootercontrold PUBLIC cxx_std_23)

# Link the scootercontrol library to the audioscootercontrold executable
target_link_libraries(visualscootercontrold PUBLIC
    scootercontrol
)

# Build the ws281x library
add_subdirectory(../../libs/rpi_ws281x ws2811)

# Link the ws281x library to the visualscootercontrold executable
target_link_libraries(visualscootercontrold PUBLIC
    ws2811
)

# Include the ws281x library's include directory
target_include_directories(visualscootercontrold PUBLIC
    ../../libs/rpi_ws281x
)
