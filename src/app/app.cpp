#include "app.h"
#include "file_system.h"
#include "watch.h"
#include "log.h"
#include <iostream>
#include <string>
#include <format>
#include <windows.h>
// The WNDPROC callback function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE: // window created
            MessageBoxW(hwnd, L"Window Created!", L"DL-Tidy", MB_OK);
            
            break;

        case WM_TRAYICON: // tray icon clicked
            if (lParam == WM_RBUTTONUP) {
                MessageBoxW(hwnd, L"Right-clicked!", L"DL-Tidy", MB_OK);
            }
            break;

        case WM_DESTROY:
            MessageBoxW(hwnd, L"destroyed", L"DL-Tidy", MB_OK);  
            PostQuitMessage(0);
            break;

        default:
            // For any messages not explicitly handled, pass them to the default window procedure
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0; // Message handled
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int) {

    App app;

    if (!app.init(hInst)) {
        MessageBoxW(nullptr, L"Failed to application", L"Error", MB_OK);
        return -1;
    }

    app.run();
    
    return 0;
}

App::App() {

}

App::~App() {

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
                            nullptr, nullptr, hInst, nullptr);

                    
    if (!hWnd_) return false;

    if (!trayApp_.init(hWnd_)) return false;

    return true;
}

void App::run() {
    // Message loop: REQUIRED so the icon can receive clicks and the app stays alive
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void App::hideWindow() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

