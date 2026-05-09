#pragma once

#include <windows.h>
#include <filesystem>
#include <chrono>
#include <atomic>

#include "file_change_handler_interface.h"

/*
    Monitors the specified directory for file system changes.

    @param hDir   Handle to an open directory obtained via CreateFileW; used by the 
                  function to receive change notifications and control the watcher thread.
*/

class EventWatcher {
    private:    
        HANDLE hIOCP_;
        HANDLE hDir_;
        IFileChangeHandler* handler_;
        std::atomic<bool> isFinished_ {false};

    public:
        EventWatcher(IFileChangeHandler* handler, HANDLE hDir) : handler_(handler), hDir_(hDir) {}
        ~EventWatcher();
        void init();
        void watch(std::filesystem::path path);
        void stop();

        template <typename Clock, typename Duration>
        static bool isReadyToRun(std::chrono::time_point<Clock, Duration> start,
                                 std::chrono::duration<double> interval);
};
