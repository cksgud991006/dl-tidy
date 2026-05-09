#pragma once

#include <windows.h>
#include <thread>
#include <memory>
#include <vector>

#include "tray_app.h"
#include "event_watcher.h"
#include "file_change_handler_interface.h"
#include "shared_directory_manager.h"

class App {
    private:
        TrayApp trayApp_;
        WNDCLASSW wc;

        std::shared_ptr<EventWatcher> eventWatcher_;
        std::shared_ptr<SharedDirectoryManager> eventHandler_;

        std::thread watchThread_; 
        std::vector<std::thread> jobThreads_;
        std::atomic<bool> watchThreadRunning_;
        
        HWND hWnd_; // void*
        HANDLE hDir_; // void*

        void runMessageLoop();
        void runFileWatcher();
        void runFileJobExecutor();

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        App();
        ~App();
        bool init(HINSTANCE hInst);
        void showTrayMenu();
        void hideWindow();
        void runTasks();
        void stop();
};