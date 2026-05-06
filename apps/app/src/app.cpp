#include <windows.h>
#include <winuser.h>

#include "app.h"
#include "directory.h"
#include "watch.h"
#include "clean.h"
#include "tray_resource.h"

/*
    Public Functions
*/

App::App() {

}

App::~App() {

}

bool App::init(HINSTANCE hInst) {

    wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"DLTidyHiddenClass";
    
    ATOM atom = RegisterClassW(&wc);

    if (atom == 0) return false;

    HWND hWnd = CreateWindowExW(WS_EX_TOOLWINDOW, // hidden window (no taskbar, no Alt-Tab)
                            wc.lpszClassName, L"DL-Tidy Window",
                            WS_POPUP,   // exists outside an application's main window
                            0, 0, 0, 0, // position/size 0 → hidden 
                            nullptr, nullptr, hInst, this);

                    
    if (!hWnd) return false;

    if (!trayApp_.init(hWnd)) return false;

    return atom != 0;
}

void App::stop() {
    
    CloseHandle(watcherThreadHandle_);

    if (watchThread_.joinable()) watchThread_.join();

}

void App::hideWindow() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

void App::showTrayMenu() {
    trayApp_.showMenu();
}

void App::removeTrayResources() {
    trayApp_.stop();
}


void App::runTasks() {
    runFileWatcher();
    runMessageLoop();
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

/*
    The WNDPROC callback
*/ 
LRESULT CALLBACK App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    App* pThis;

    switch (uMsg)
    {
        // window constructor
        case WM_CREATE: { 
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<App*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            break;
        }

        case WM_TRAYICON: {

            LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
            pThis = reinterpret_cast<App*>(data);

            if (lParam == WM_RBUTTONUP) {
                pThis->showTrayMenu();

                PostMessage(hwnd, WM_NULL, 0, 0); // let the menu close properly
            }

            break;
        }

        case WM_COMMAND: {// dropdown command

            LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
            pThis = reinterpret_cast<App*>(data);

            switch(LOWORD(wParam)) {

                case ID_TRAY_OPEN_LOG_LOCATION: {

                    std::filesystem::path logPath = getLogPath();
                    LPCWSTR lpcwstr = logPath.c_str();

                    ShellExecuteW(NULL, L"open", lpcwstr, NULL, NULL, SW_SHOWNORMAL);
                    
                    break;
                }

                case ID_TRAY_CLEAN: {

                    cleanUp(getDownloadsPath());
                    
                    break;
                }

                case ID_TRAY_EXIT: {

                    pThis->removeTrayResources();
                    
                    pThis->stop();

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