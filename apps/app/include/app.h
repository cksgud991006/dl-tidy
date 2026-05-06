#pragma once

#include <windows.h>
#include <thread>
#include "tray_app.h"

class App {
    private:
        TrayApp trayApp_;
        WNDCLASSW wc;

        std::thread watchThread_; 
        std::atomic<bool> watchThreadRunning_;
        HANDLE watcherThreadHandle_;
        
        void runMessageLoop();
        void runFileWatcher();

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        App();
        ~App();
        bool init(HINSTANCE hInst);
        void showTrayMenu();
        void removeTrayResources();
        void hideWindow();
        void runTasks();
        void stop();
};