#include "watch.h"
#include "clean.h"
#include "log.h"

void watch(std::filesystem::path path) {
    
    bool running = true;

    auto lastEventTs = std::chrono::system_clock::now();

    auto timeBound = std::chrono::milliseconds(100);

    log("Program Started");

    while (running) {

        if (isReadyToRun(lastEventTs, timeBound)) {
            
            cleanUp(path);

            lastEventTs = std::chrono::system_clock::now();        

        }

    }

    log("Program Done");
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
