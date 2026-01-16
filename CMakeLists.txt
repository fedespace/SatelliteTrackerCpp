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