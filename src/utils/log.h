#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>

inline void log(const std::string& message) {
    std::ofstream logFile("app.log", std::ios::app);
    if (!logFile) return;

    // Add timestamp
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "[" << std::put_time(std::localtime(&now), "%F %T") << "] "
            << message << '\n';
}