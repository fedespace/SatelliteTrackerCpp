# Set minimum version for compatibility reasons
cmake_minimum_required(VERSION 3.16)

# Detail the project we are going to build and the lang
project(SatelliteTracker LANGUAGES CXX)

# Defines the std version of the language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g")

# Add the executable files (list will build up)
add_executable(SatelliteTracker 
    src/main.cpp
    src/domain/TimeUtils.cpp
    src/domain/Tle.cpp
    src/propagation/Sgp4Propagator.cpp
)

# Add libraries (Vallado's SGP4 propagator), using 3 steps:

# SGP4 is its own library
add_library(sgp4
    external/sgp4/sgp4.cpp
)
set_source_files_properties(
    external/sgp4/SGP4.cpp
    PROPERTIES COMPILE_FLAGS "-fno-sanitize=address"
)

# The app is linked to it
target_include_directories(sgp4 PUBLIC external/sgp4)
target_link_libraries(SatelliteTracker PRIVATE sgp4)