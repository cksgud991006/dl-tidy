#include "tray_app.h"

class App {

    TrayApp trayApp_;
    HWND hWnd_;
    
    void hideWindow();

    public:
        App();
        ~App();
        bool init(HINSTANCE hInst);
        void run();
};