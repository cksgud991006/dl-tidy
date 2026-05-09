add_library(core STATIC 
    src/shared_directory_manager.cpp
    src/event_watcher.cpp
)

target_include_directories(core PUBLIC include)

target_link_libraries(core PRIVATE utils)