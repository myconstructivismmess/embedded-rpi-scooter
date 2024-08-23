# Set cmake minimum required version
cmake_minimum_required(VERSION 3.15)

# Set the project name and version
project(ScooterControl VERSION 1.0
                       DESCRIPTION "Scooter Control Daemons responsible for controlling the audio and visual aspects of the scooter"
                       LANGUAGES CXX)

# Make the static library used by the daemons
add_subdirectory(libs/scootercontrol)

# Add subdirectories for daemons
add_subdirectory(daemons/audioscootercontrold)
add_subdirectory(daemons/visualscootercontrold)



# # List all source files in the scootercontrol library
# file(GLOB_RECURSE SCOOTERCONTROL_SRC
#     ${CMAKE_CURRENT_SOURCE_DIR}/libs/scootercontrol/button/*.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/libs/scootercontrol/button/handlers/*.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/libs/scootercontrol/button/interceptors/*.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/libs/scootercontrol/utils/*.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/libs/scootercontrol/timers/*.cpp
# )

# # Add the scootercontrol library
# add_library(scootercontrol STATIC ${SCOOTERCONTROL_SRC})

# # Specify the include directories
# target_include_directories(scootercontrol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/scootercontrollib)

# # Find and link the wiringPi library
# find_library(WIRINGPI_LIB wiringPi REQUIRED)

# # Include the wiringPi library
# include_directories(
#     include
#     ${wiringPi_INCLUDE_DIRS}
# )

# # Link wiringPi to the scootercontrol library
# target_link_libraries(scootercontrol ${WIRINGPI_LIB})
