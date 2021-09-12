# The MIT License (MIT)
#
# Copyright (c) 2017-2021 Alexander Kurbatov

cmake_minimum_required(VERSION 3.1)

project(Suvorov)

set(PROJECT_AUTHOR alkurbatov)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 0)

# Specify output directories.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# Build with c++14 support, required by sc2api.
set(CMAKE_CXX_STANDARD 14)

# Dont build examples in the sc2api submodule.
set(BUILD_API_EXAMPLES OFF CACHE INTERNAL "" FORCE)

# Dont build tests in the sc2api submodule.
set(BUILD_API_TESTS OFF CACHE INTERNAL "" FORCE)

# Dont build sc2renderer in the sc2api submodule.
set(BUILD_SC2_RENDERER OFF CACHE INTERNAL "" FORCE)

option(BUILD_FOR_LADDER "Create build for the AIArena ladder" OFF)

add_subdirectory("contrib/cpp-sc2")
add_subdirectory("src")
