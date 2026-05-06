cmake_minimum_required(VERSION 3.20)
project(dl_tidy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(apps)

# Link resources
target_sources(Dl-Tidy PRIVATE resources.rc)

# If you're using MinGW (not MSVC), ensure Unicode entry point
if (MINGW)
  target_link_options(Dl-Tidy PRIVATE -municode)
endif()