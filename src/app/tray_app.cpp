#include "tray_app.h"
#include "resources.h"
#include <strsafe.h>
#include "log.h"
#include <format>
#include "types.h"

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
    nid_.hIcon = LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDI_DLTIDY_ICON));

    AddTrayIcon();

    setVersion();
    
    setTooltipText(L"Dl-Tidy");

    if (!initTrayMenu(hwnd)) return false;

    return true;

}

void TrayApp::setTooltipText(const wchar_t* text) {

    StringCchCopyW(nid_.szTip, ARRAYSIZE(nid_.szTip), text);

    Shell_NotifyIconW(NIM_MODIFY, &nid_);

}

void TrayApp::AddTrayIcon() {

    Shell_NotifyIconW(NIM_ADD, &nid_);

}


void TrayApp::setVersion() {

    Shell_NotifyIconW(NIM_SETVERSION, &nid_);

}

bool TrayApp::initTrayMenu(HWND hwnd) {
    trayMenu_ = CreatePopupMenu();
    if (!trayMenu_) return false;

    // Build the menu
    AppendMenuW(trayMenu_, MF_STRING, ID_TRAY_CLEAN, L"Clean");
    AppendMenuW(trayMenu_, MF_STRING, ID_TRAY_EXIT,  L"Exit");

    SetMenu(hwnd, trayMenu_);

    return true;
}

void TrayApp::showTrayMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt);         // current cursor position (screen coords)
    SetForegroundWindow(hwnd); // required for proper focus and auto-dismiss
    TrackPopupMenu(
        trayMenu_,              // the HMENU you created earlier
        TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON,
        pt.x, pt.y, 0,
        hwnd, nullptr
    );
}

void TrayApp::removeTrayResources() {

    if (nid_.cbSize) {
        Shell_NotifyIconW(NIM_DELETE, &nid_);
        nid_ = {};
    }
}