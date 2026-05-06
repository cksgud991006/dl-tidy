#include "hidden_window.h"
#include "app.h"
#include "directory.h"
#include "clean.h"
#include "tray_resource.h"
/*
    Public Functions
*/

bool Window::init(HINSTANCE hInst) {
    wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"DLTidyHiddenClass";
    
    ATOM atom = RegisterClassW(&wc);

    return atom != 0;
}

void Window::hideWindow() {
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

/*
    Private Functions
*/

// The WNDPROC callback function
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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