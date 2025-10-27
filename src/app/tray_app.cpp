#include "tray_app.h"
#include "resources.h"
#include <strsafe.h>
#include "log.h"
#include <format>

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

void TrayApp::shutdown() {

    if (nid_.cbSize) {
        Shell_NotifyIconW(NIM_DELETE, &nid_);
        nid_ = {};
    }

}