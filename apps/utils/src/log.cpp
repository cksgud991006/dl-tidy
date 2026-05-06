#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "directory.h"
#include "log.h"

void log(const std::string& message) {
    
    std::filesystem::path logDir = getLogPath();
    
    std::filesystem::create_directories(logDir);

    std::filesystem::path logFilePath = logDir / "log.log";

    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) return;

    // Add timestamp
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "[" << std::put_time(std::localtime(&now), "%F %T") << "] "
            << message << '\n';
}

void log_debug(const std::string& message) {
    
    std::filesystem::path logDir = getLogPath();
    
    std::filesystem::create_directories(logDir);

    std::filesystem::path logFilePath = logDir / "log.log";

    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) return;

    // Add timestamp
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "[" << std::put_time(std::localtime(&now), "%F %T") << "] "
            << "DEBUG: " << message << '\n';
}

void log_error(const std::string& message) {
    
    std::filesystem::path logDir = getLogPath();
    
    std::filesystem::create_directories(logDir);

    std::filesystem::path logFilePath = logDir / "log.log";

    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) return;

    // Add timestamp
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "[" << std::put_time(std::localtime(&now), "%F %T") << "] "
            << "ERROR: " << message << '\n';
}