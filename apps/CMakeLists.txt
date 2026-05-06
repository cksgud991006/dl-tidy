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
