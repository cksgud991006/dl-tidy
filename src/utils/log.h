#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "file_system.h"

inline void log(const std::string& message) {
    
    std::filesystem::path logDir = getLocalAppDataPath() / "DL-Tidy" / "logs";
    
    std::filesystem::create_directories(logDir);

    std::filesystem::path logFilePath = logDir / "log.log";

    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) return;

    // Add timestamp
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "[" << std::put_time(std::localtime(&now), "%F %T") << "] "
            << message << '\n';
}