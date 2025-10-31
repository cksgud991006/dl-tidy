#include <filesystem>
#include <chrono>
#include <atomic>
#include "windows.h"

/*
    Monitors the specified directory for file system changes.

    @param path   Absolute path of the directory to watch for file or folder changes.
    @param hDir   Handle to an open directory obtained via CreateFileW; used by the 
                  function to receive change notifications and control the watcher thread.
*/
void watch(std::filesystem::path path, HANDLE& hDir);

template <typename Clock, typename Duration>
bool isReadyToRun(std::chrono::time_point<Clock, Duration> start,
                  std::chrono::duration<double> interval);