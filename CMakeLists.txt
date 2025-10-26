cmake_minimum_required(VERSION 3.20)
project(dl_tidy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all sources
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/core/clean.cpp
    ${SRC_DIR}/core/file_system.cpp
    ${SRC_DIR}/watch/watch.cpp
)

# Create executable
add_executable(dl-tidy ${SOURCES})

# Include directories for headers
target_include_directories(dl-tidy PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/core
    ${SRC_DIR}/watch
    ${SRC_DIR}/utils
)

