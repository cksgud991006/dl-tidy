#pragma once

#include <windows.h>
#include <thread>

#include "tray_app.h"
#include "hidden_window.h"


class App {
    private:
        TrayApp trayApp_;
        Window window_;

        std::thread watchThread_; 
        std::atomic<bool> watchThreadRunning_;
        HANDLE watcherThreadHandle_;
        
        void runMessageLoop();
        void runFileWatcher();

    public:
        App();
        ~App();
        bool init(HINSTANCE hInst);
        void runTasks();
        void showTrayMenu();
        void removeTrayResources();
        void stop();
};