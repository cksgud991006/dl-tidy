#include <format>
#include <thread>

#include "watch.h"
#include "clean.h"
#include "log.h"

void watch(std::filesystem::path path, HANDLE& hDir) {
    
    BYTE buf[64 * 1024]; // 64 KB is a safe common size
    DWORD bytes;

    log_debug("File watcher thread started");
    
    for (;;) {    // sync mode
        
        bool ok = ReadDirectoryChangesW(hDir, 
                buf, sizeof(buf), 
                false,                          // monitor directory itself without subfolders
                FILE_NOTIFY_CHANGE_CREATION | 
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_FILE_NAME |
                FILE_NOTIFY_CHANGE_LAST_ACCESS, 
                &bytes, nullptr, nullptr);
                
        if (!ok) {
            DWORD err = GetLastError();
            log_debug(std::format("RDCW failed: {}", err));
            if (err == ERROR_INVALID_HANDLE || err == ERROR_OPERATION_ABORTED) {
                log_debug("Watcher thread shutting down: Handle closed.");
                break;
            }
            log_error(std::format("Watcher error: {}", err));
            break;
        }

        if (bytes > 0) {
            cleanUp(path);
        }
    }

}

template <typename Clock, typename Duration>
bool isReadyToRun(std::chrono::time_point<Clock, Duration> start,
                  std::chrono::duration<double> interval)
{
    auto now = std::chrono::system_clock::now();

    if (now - start > interval) {
        return true;
    }

    return false;
}
