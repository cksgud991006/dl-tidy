add_library(core STATIC 
    src/clean.cpp
    src/watch.cpp
)

target_include_directories(core PUBLIC include)

target_link_libraries(core PRIVATE utils)