#include <filesystem>
#include <chrono>
#include <atomic>

// main task running on background
void watch(std::filesystem::path path, std::atomic<bool>& running);

template <typename Clock, typename Duration>
bool isReadyToRun(std::chrono::time_point<Clock, Duration> start,
                  std::chrono::duration<double> interval);