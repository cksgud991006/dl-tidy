# Register libraries
add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(app)

# Set source
set(SOURCE main.cpp)

# Create executable
add_executable(Dl-Tidy WIN32 ${SOURCE})

# Link libraries
target_link_libraries(Dl-Tidy PRIVATE 
    app
    core
    utils
)

# Link resources
target_sources(Dl-Tidy PRIVATE resources.rc)

if (MSVC)
    # Turn off MSVC's automatic manifest generation so it uses yours from resources.rc
    target_link_options(Dl-Tidy PRIVATE "/MANIFEST:NO")
endif()

# If you're using MinGW (not MSVC), ensure Unicode entry point
if (MINGW)
  target_link_options(Dl-Tidy PRIVATE -municode)
endif()