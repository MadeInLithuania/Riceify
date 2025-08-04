cmake_minimum_required(VERSION 3.23)
project(Riceify_)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compiler optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra")

# Enable link-time optimization for release builds
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

# Find required packages
find_package(Threads REQUIRED)

add_executable(Riceify main.cpp Headers/Navigation.h Headers/logs.h Headers/Banner.h)

# Link against threading library for async operations
target_link_libraries(Riceify Threads::Threads)

# Set compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Riceify PRIVATE -Wall -Wextra -Wpedantic)
endif()
