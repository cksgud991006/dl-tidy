#include <windows.h>
#include <winuser.h>

#include "app.h"
#include "directory.h"
#include "tray_resource.h"
#include "shared_directory_manager.h"
#include "log.h"

/*
    Public Functions
*/

App::App() {

}

App::~App() {
    stop();
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
    
    log_debug("Stopping app...");

    if (eventWatcher_) eventWatcher_->stop();
    if (eventHandler_) eventHandler_->stop();

    if (watchThread_.joinable()) watchThread_.join();
    if (jobThread_.joinable()) jobThread_.join();

    trayApp_.stop();

    PostQuitMessage(0);

    log_debug("App stopped");

}

void App::hideWindow() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

void App::showTrayMenu() {
    trayApp_.showMenu();
}

void App::runTasks() {
    runFileWatcher();
    runFileJobExecutor();
    runMessageLoop();
}

/*
    Private Functions
*/

void App::runFileWatcher() { 
    
    auto const path = getDownloadsPath();

    HANDLE hDir = CreateFileW(
                        path.c_str(),                  // Directory path
                        FILE_LIST_DIRECTORY,           // Desired access
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // Share mode
                        nullptr,                       // Security attributes
                        OPEN_EXISTING,                 // Creation disposition
                        FILE_FLAG_BACKUP_SEMANTICS,    // Flags & attributes
                        nullptr                        // Template file (unused)
    );
    
    eventHandler_ = std::make_shared<SharedDirectoryManager>();
    eventWatcher_ = std::make_shared<EventWatcher>(eventHandler_.get(), hDir);

    watchThread_ = std::thread(&EventWatcher::watch, eventWatcher_.get(), path);

}

void App::runFileJobExecutor() {
    jobThread_ = std::thread(&SharedDirectoryManager::execute, eventHandler_.get());
}

void App::runMessageLoop() {
    // TODO: stop loop preperly
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
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

                    pThis->eventHandler_.get()->scheduleFilesJob(getDownloadsPath());
                    
                    break;
                }

                case ID_TRAY_EXIT: {
                    
                    pThis->stop();

                    break;
                }
            }

            break;
        }

        case WM_DESTROY: {

            LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);

            pThis = reinterpret_cast<App*>(data);

            pThis->stop();

            break;
        }

        default: {
            // For any messages not explicitly handled, pass them to the default window procedure
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return 0; // Message handled
}