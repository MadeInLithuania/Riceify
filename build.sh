#!/bin/bash

# Riceify Performance Build Script
# This script builds the project with different optimization levels

set -e

echo "=== Riceify Performance Build Script ==="

# Create build directory
mkdir -p build
cd build

# Function to build with specific configuration
build_config() {
    local config=$1
    local build_dir=$2
    
    echo "Building with configuration: $config"
    mkdir -p "$build_dir"
    cd "$build_dir"
    
    cmake -DCMAKE_BUILD_TYPE="$config" ../..
    make -j$(nproc)
    
    echo "Build completed for $config in $build_dir/"
    cd ..
}

# Build debug version
build_config "Debug" "debug"

# Build release version with optimizations
build_config "Release" "release"

# Build with maximum optimizations
echo "Building with maximum optimizations..."
mkdir -p max_opt
cd max_opt

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG -march=native -flto" ../..
make -j$(nproc)

echo "Build completed for maximum optimization in max_opt/"
cd ..

echo ""
echo "=== Build Summary ==="
echo "Debug build:     build/debug/Riceify"
echo "Release build:   build/release/Riceify"
echo "Max optimized:   build/max_opt/Riceify"
echo ""
echo "To run a specific build:"
echo "  ./build/debug/Riceify     # For debugging"
echo "  ./build/release/Riceify   # For production use"
echo "  ./build/max_opt/Riceify   # For maximum performance"
