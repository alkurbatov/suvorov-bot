cmake_minimum_required(VERSION 3.1)

project(Suvorov)

# Specify output directories.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

find_package(Boost 1.58 REQUIRED log log_setup system thread)

# Build with c++14 support, required by sc2api.
set(CMAKE_CXX_STANDARD 14)

add_subdirectory("s2client-api")
add_subdirectory("src")
