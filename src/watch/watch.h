#include <filesystem>
#include <chrono>

// main task running on background
void watch(std::filesystem::path path);

template <typename Clock, typename Duration>
bool isReadyToRun(std::chrono::time_point<Clock, Duration> start,
                  std::chrono::duration<double> interval);