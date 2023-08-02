#!/bin/bash

# Define the build directory
build_dir="./out/build"

# Remove the existing build directory
rm -rf "$build_dir"

# Create the new build directory, including any necessary parent directories
mkdir -p "$build_dir"

# Change to the new build directory
cd "$build_dir"

# Run CMake, assuming the source directory is two levels up
cmake ../..

# Run make to build the project
make

# Exit with the status of the make command
exit $?
