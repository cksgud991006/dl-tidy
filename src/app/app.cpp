#include "app.h"
#include "file_system.h"
#include "watch.h"
#include "log.h"
#include <iostream>
#include <string>
#include <format>
#include <windows.h>
#include "types.h"
#include "clean.h"

App* pThis = nullptr;

// The WNDPROC callback function
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE: { // window created 
            // Get 'this' pointer from CREATESTRUCT and store in GWLP_USERDATA
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<App*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            break;
        }
        case WM_TRAYICON: {// tray icon clicked  
            if (lParam == WM_RBUTTONUP) {
                pThis->showTrayMenu();

                PostMessage(hwnd, WM_NULL, 0, 0); // let the menu close properly
            }

            break;
        }

        case WM_COMMAND: {// dropdown command
            switch(LOWORD(wParam)) {
                case ID_TRAY_CLEAN: {

                    cleanUp(getDownloadsPath());
                    
                    break;
                }

                case ID_TRAY_EXIT: {
                    pThis->removeUIResources();
                    
                    pThis->stop();

                    DestroyWindow(hwnd);

                    break;
                }
            }

            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);

            break;
        }

        default: {
            // For any messages not explicitly handled, pass them to the default window procedure
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return 0; // Message handled
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int) {

    App app;

    if (!app.init(hInst)) {
        MessageBoxW(nullptr, L"Failed to application", L"Error", MB_OK);
        return -1;
    }

    // run tasks
    app.runFileWatcher();
    app.runMessageLoop();

    return 0;
}

App::App() {

}

App::~App() {
    stop();
}

bool App::init(HINSTANCE hInst) {

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"DLTidyHiddenClass";
    RegisterClassW(&wc);

    hWnd_ = CreateWindowExW(WS_EX_TOOLWINDOW, // hidden window (no taskbar, no Alt-Tab)
                            wc.lpszClassName, L"DL-Tidy Window",
                            WS_POPUP,   // exists outside an application's main window
                            0, 0, 0, 0, // position/size 0 → hidden 
                            nullptr, nullptr, hInst, this);

                    
    if (!hWnd_) return false;

    if (!trayApp_.init(hWnd_)) return false;

    return true;
}

void App::stop() {
    
    free(pThis);

    CloseHandle(watcherThreadHandle_);

    if (watchThread_.joinable()) watchThread_.join();

}

/*
    Thread Works
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

/*
    UI works
*/

void App::hideWindow() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

void App::showTrayMenu() {
    trayApp_.showTrayMenu(hWnd_);
}

void App::removeUIResources() {
    trayApp_.removeTrayResources();
}