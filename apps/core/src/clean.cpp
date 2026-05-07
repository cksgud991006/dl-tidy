#include <format>

#include "clean.h"
#include "log.h"

std::uintmax_t cleanUp(std::filesystem::path path) {
    
    std::uintmax_t nums = 0;
    std::error_code ec;

    log_debug(std::format("Cleaning up {}", path.string()));

    try {
        nums = std::filesystem::remove_all(path);
    } catch (const std::filesystem::filesystem_error& e) {
        log_error(e.what());
        log_error(e.path1().string());
    }

    log_debug(std::format("Result: Deleted {} files", nums));

    return nums;
}

bool clean(std::filesystem::path path) {
    
    bool success;

    success = std::filesystem::remove(path);

    return success;
}