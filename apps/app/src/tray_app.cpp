#include <windows.h>   // MUST BE FIRST
#include <shellapi.h>  // Required for Shell_NotifyIcon and NOTIFYICONDATA
#include <strsafe.h>
#include <format>

#include "tray_app.h"
#include "log.h"


/*
    Public Functions
*/

TrayApp::TrayApp() {

}

TrayApp::~TrayApp() {

}

bool TrayApp::init(HWND hwnd) {
    
    nid_ = {};
    nid_.cbSize = sizeof(nid_);
    nid_.hWnd = hwnd;
    nid_.uID = 1;
    nid_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid_.uCallbackMessage = WM_TRAYICON;
    nid_.hIcon = LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDI_DLTIDY_ICON_NAME));

    hWnd_ = hwnd;

    AddTrayIcon();

    setVersion();
    
    setTooltipText(L"Dl-Tidy");

    if (!initTrayMenu()) return false;

    return true;

}

/*
    Private Functions
*/

void TrayApp::AddTrayIcon() {

    Shell_NotifyIconW(NIM_ADD, &nid_);

}


void TrayApp::setVersion() {

    Shell_NotifyIconW(NIM_SETVERSION, &nid_);

}

bool TrayApp::initTrayMenu() {
    trayMenu_ = CreatePopupMenu();
    if (!trayMenu_) return false;

    // Build the menu
    AppendMenuW(trayMenu_, MF_STRING, ID_TRAY_OPEN_LOG_LOCATION, L"Open Logs");
    AppendMenuW(trayMenu_, MF_STRING, ID_TRAY_CLEAN, L"Clean");
    AppendMenuW(trayMenu_, MF_STRING, ID_TRAY_EXIT,  L"Exit");

    return true;
}

void TrayApp::showMenu() {
    POINT pt;
    GetCursorPos(&pt);         // current cursor position (screen coords)
    SetForegroundWindow(hWnd_); // required for proper focus and auto-dismiss
    TrackPopupMenu(
        trayMenu_,              // the HMENU you created earlier
        TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON,
        pt.x, pt.y, 0,
        hWnd_, nullptr
    );
}

void TrayApp::setTooltipText(const wchar_t* text) {

    StringCchCopyW(nid_.szTip, ARRAYSIZE(nid_.szTip), text);

    Shell_NotifyIconW(NIM_MODIFY, &nid_);

}

void TrayApp::stop() {

    log_debug("Stopping tray app...");

    if (nid_.cbSize) {
        Shell_NotifyIconW(NIM_DELETE, &nid_);
        nid_ = {};
    }

    DestroyMenu(trayMenu_);

    log_debug("Tray app stopped");
}

