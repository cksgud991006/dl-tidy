#include "tray_app.h"
#include <atomic>
#include <thread>

class App {

    TrayApp trayApp_;
    std::thread watchThread_; 
    std::atomic<bool> watchThreadRunning_;
    HWND hWnd_;
    HANDLE watcherThreadHandle_;

    public:
        App();
        ~App();
        bool init(HINSTANCE hInst);
        
        /*
            Thread Works
        */
        void runMessageLoop();
        void runFileWatcher();
        void stop();
        /*
            UI Works
        */
        void showTrayMenu();
        void hideWindow();
        void removeUIResources();
};