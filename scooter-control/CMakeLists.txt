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
