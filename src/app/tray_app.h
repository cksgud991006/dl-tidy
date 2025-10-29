#include <windows.h>
#include <shellapi.h>
#include <string>

class TrayApp {
    NOTIFYICONDATAW nid_;
    HMENU trayMenu_;

    void AddTrayIcon();
    void setTooltipText(const wchar_t* text);
    void setVersion();
    bool initTrayMenu(HWND hwnd);

    public:
        TrayApp();
        ~TrayApp();
        bool init(HWND hwnd);
        void showTrayMenu(HWND hwnd);
        void removeTrayResources();
        
};