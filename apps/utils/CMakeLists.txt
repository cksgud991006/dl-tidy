add_library(utils STATIC 
    src/log.cpp
    src/directory.cpp
)

target_include_directories(utils PUBLIC include)