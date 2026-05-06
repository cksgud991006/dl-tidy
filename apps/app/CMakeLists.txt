add_library(app STATIC
    src/app.cpp
    src/tray_app.cpp
    src/hidden_window.cpp
)

target_include_directories(app PUBLIC include)

target_link_libraries(app PRIVATE 
    core
    utils
    watch
)