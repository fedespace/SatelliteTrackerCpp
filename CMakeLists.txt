# Set minimum version for compatibility reasons
cmake_minimum_required(VERSION 3.16)

# Detail the project we are going to build and the lang
project(SatelliteTracker LANGUAGES CXX)

# Defines the std version of the language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable files (list will build up)
add_executable(SatelliteTracker src/main.cpp
)

# Add libraries (Vallado's SGP4 propagator), using 3 steps:

# SGP4 is its own library
add_library(sgp4
    external/sgp4/sgp4.cpp
)

# The app is linked to it
target_include_directories(sgp4 PUBLIC external/sgp4)
target_link_libraries(SatelliteTracker PRIVATE sgp4)