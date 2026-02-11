#!/bin/bash

building_dir="./build/"
project="SatelliteTracker"

if [ ! -d "$building_dir" ]; then
    echo "Building directory does not exist. Proceeding to create it."
    cmake -S . -B "$building_dir"
fi

echo "Running the project..."
cd .
cmake --build "$building_dir"
cd "$building_dir"
./"$project"