#pragma once

#include <windows.h>
#include <shellapi.h>
#include <string>

#include "tray_resource.h"

enum COMMAND {
    ID_TRAY_OPEN_LOG_LOCATION,
    ID_TRAY_CLEAN,
    ID_TRAY_EXIT
};

class TrayApp {
    private: 
        HWND hWnd_;
        NOTIFYICONDATAW nid_;
        HMENU trayMenu_;

        bool initTrayMenu();
        void AddTrayIcon();
        void setVersion();
        void setTooltipText(const wchar_t* text);
        
    public:
        TrayApp();
        ~TrayApp();
        bool init(HWND hwnd);
        void showMenu();
        void stop();
};