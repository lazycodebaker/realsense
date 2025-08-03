#!/bin/bash

# Exit immediately if any command fails
set -e

rm -rf build

# Create build directory and navigate into it
mkdir -p build
cd build

# Run cmake and build
cmake ..
cmake --build . --parallel
cmake --build . --target run

cd ..
