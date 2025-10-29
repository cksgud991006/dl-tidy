cmake_minimum_required(VERSION 3.20)
project(dl_tidy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all sources
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    ${SRC_DIR}/app/app.cpp
    ${SRC_DIR}/app/tray_app.cpp
    ${SRC_DIR}/core/clean.cpp
    ${SRC_DIR}/core/file_system.cpp
    ${SRC_DIR}/watch/watch.cpp
)

# Create executable
add_executable(Dl-Tidy WIN32 ${SOURCES})

# Include directories for headers
target_include_directories(Dl-Tidy PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/app
    ${SRC_DIR}/core
    ${SRC_DIR}/watch
    ${SRC_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link resources
target_sources(Dl-Tidy PRIVATE resources.rc)

# If you're using MinGW (not MSVC), ensure Unicode entry point
if (MINGW)
  target_link_options(Dl-Tidy PRIVATE -municode)
endif()