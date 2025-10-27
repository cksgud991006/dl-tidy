#include <windows.h>
#include <shellapi.h>
#include <string>

#define WM_TRAYICON (WM_APP + 1)

class TrayApp {
    NOTIFYICONDATAW nid_;
    
    void AddTrayIcon();
    void setTooltipText(const wchar_t* text);
    void setVersion();
    void shutdown();

    public:
        TrayApp();
        ~TrayApp();
        bool init(HWND hwnd);
};