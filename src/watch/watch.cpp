#include "watch.h"
#include "clean.h"
#include "log.h"
#include "windows.h"
#include <thread>

void watch(std::filesystem::path path, std::atomic<bool>& running) {

    while (running) {
        
        cleanUp(path);

        std::this_thread::sleep_for(std::chrono::minutes(10)); 
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
