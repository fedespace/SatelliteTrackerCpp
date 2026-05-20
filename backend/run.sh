#!/bin/bash

# Option 1 - re-creation of the build folder is not necessary (no changes in CMakeLists.txt)
# Option 2 - the build folder needs to be deleted and re-created (changes made in CMakeLists.txt)
# default: rebuild=false
# default: rebuild=true if any changes have been made to CMakeLists.txt

# INITIAL VARIABLES
rebuild=false
building_dir="./build/"
project="SatelliteTracker"

# INITIALISE CMAKELISTS FILE AND ITS HASHFILE PATH
file="CMakeLists.txt"
hashfile="${file}.sha256"

# COMPUTE THE CURRENT HASH 
new_hash=$(sha256sum "$file" | awk '{print $1}')

# CHECK IF A PREVIOUS HASHFILE WAS ALREADY PRESENT AND IN CASE FORCE AUTOMATIC REBUILD
if [[ -f "$hashfile" ]]; then
  old_hash=$(<"$hashfile")
  if [[ "$new_hash" != "$old_hash" ]]; then
    rebuild=true
  fi
fi

# IF REBUILD IS MANUALLY PROMPTED
for arg in "$@"; do
  if [[ "$arg" == "--rebuild" ]]; then
    rebuild=true
    break
  fi
done

# IN THE TWO CASES WHERE REBUILD IS TRUE (AUTOMATIC OR MANUAL)
if $rebuild; then
    echo "Rebuilding the project..."
    if [ -d "$building_dir" ]; then 
        rm -rf "$building_dir"
    fi
    cmake -S . -B "$building_dir"
fi

# COMMON CODE FOR BOTH CASES
cd .
cmake --build "$building_dir"
cd "$building_dir"
./"$project"