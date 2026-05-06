#include <windows.h>

#include "app.h"

/*
    Public Functions
*/

App::App() {

}

App::~App() {

}

bool App::init(HINSTANCE hInst) {

    hWnd = CreateWindowExW(WS_EX_TOOLWINDOW, // hidden window (no taskbar, no Alt-Tab)
                            wc.lpszClassName, L"DL-Tidy Window",
                            WS_POPUP,   // exists outside an application's main window
                            0, 0, 0, 0, // position/size 0 → hidden 
                            nullptr, nullptr, hInst, this);

                    
    if (!hWnd) return false;

    if (!window_.init(hInst)) return false;

    if (!trayApp_.init(hWnd)) return false;

    return true;
}

void App::stop() {
    
    CloseHandle(watcherThreadHandle_);

    if (watchThread_.joinable()) watchThread_.join();

}


void App::runTasks() {
    runFileWatcher();
    runMessageLoop();
}

void App::showTrayMenu() {
    trayApp_.showMenu();
}

void App::removeTrayResources() {
    trayApp_.stop();
}

/*
    Private Functions
*/
void App::runMessageLoop() {
    // Message loop: REQUIRED so the icon can receive clicks and the app stays alive
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void App::runFileWatcher() {
    
    auto const path = getDownloadsPath();

    watcherThreadHandle_ = CreateFileW(
                        path.c_str(),                  // Directory path
                        FILE_LIST_DIRECTORY,           // Desired access
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // Share mode
                        nullptr,                       // Security attributes
                        OPEN_EXISTING,                 // Creation disposition
                        FILE_FLAG_BACKUP_SEMANTICS,    // Flags & attributes
                        nullptr                        // Template file (unused)
                    );
    
    watchThread_ = std::thread(&watch, path, std::ref(watcherThreadHandle_));

}