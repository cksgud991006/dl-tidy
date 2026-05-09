#include <format>
#include <thread>

#include "event_watcher.h"
#include "log.h"


EventWatcher::~EventWatcher() {

    stop();

    if (hIOCP_) CloseHandle(hIOCP_);
    if (hDir_) CloseHandle(hDir_);

}

void EventWatcher::init() {
    hIOCP_ = CreateIoCompletionPort(hDir_, NULL, 0, 1);
}


void EventWatcher::watch(std::filesystem::path path) {
        
    if (!hIOCP_ || hIOCP_ == INVALID_HANDLE_VALUE) { 
        log_error(std::format("Failed to create IOCP: {}", GetLastError()));
        return;
    }

    log_debug("Event watcher thread started");

    bool success;

    BYTE buf[1024 * 16]; // 16 kb buffer for change notifications

    for (;;) {
        
        // Reset overlapped for every new request
        memset(&overlapped_rdc, 0, sizeof(OVERLAPPED));

        success = ReadDirectoryChangesW(hDir_, 
                buf, sizeof(buf), 
                false,                          // monitor directory itself without subfolders
                FILE_NOTIFY_CHANGE_CREATION | 
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_FILE_NAME |
                FILE_NOTIFY_CHANGE_LAST_ACCESS, 
                NULL, &overlapped_rdc, NULL);
                
        if (!success) {
            DWORD err = GetLastError();
            log_error(std::format("RDCW failed: {}", err));
            break;
        }

        DWORD bytes;
        ULONG_PTR completionKey;
        LPOVERLAPPED overlapped_gqcs;

        success = GetQueuedCompletionStatus(hIOCP_, &bytes, &completionKey, &overlapped_gqcs, INFINITE);
        
        if (completionKey == 0xDEAD || isFinished_) {
            log_debug("File watcher thread stopping...");
            break;
        }

        if (success && bytes > 0) {
            
            log_debug(std::format("Change detected: {} bytes", bytes));
            handler_->onFileChange(path);

        } else if (!success) {
            DWORD err = GetLastError();
            log_error(std::format("GetQueuedCompletionStatus failed: {}", err));
            break;
        }
    }

}

void EventWatcher::stop() {

    log_debug("Stopping event watcher...");

    isFinished_ = true;

    // manually post a completion status to unblock the thread if it's waiting
    PostQueuedCompletionStatus(hIOCP_, 0, 0xDEAD, NULL);
    
    log_debug("Event watcher stopped");
}


template <typename Clock, typename Duration>
bool EventWatcher::isReadyToRun(std::chrono::time_point<Clock, Duration> start,
                  std::chrono::duration<double> interval)
{
    auto now = std::chrono::system_clock::now();

    if (now - start > interval) {
        return true;
    }

    return false;
}
