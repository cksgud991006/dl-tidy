#include <format>

#include "clean.h"
#include "log.h"

std::uintmax_t cleanUp(std::filesystem::path path) {

    // manually delete files and prevent dealing with removal of downloads directory
    std::uintmax_t nums = 0;
    std::uintmax_t num;
    std::error_code ec;

    try {
        nums = std::filesystem::remove_all(path, ec);
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