#include "clean.h"
#include "log.h"
#include <format>

std::uintmax_t cleanUp(std::filesystem::path path) {

    // manually delete files and prevent dealing with removal of downloads directory

    std::uintmax_t nums = 0;
    std::uintmax_t num;

    for (const auto & file: std::filesystem::directory_iterator(path)) {
        num = std::filesystem::remove_all(file.path());
        nums += num;
        log(std::format("Cleaned {} files", num));
    }

    log(std::format("Result: Deleted {} files", nums));

    return nums;
}

bool clean(std::filesystem::path path) {
    
    bool success;

    success = std::filesystem::remove(path);

    return success;
}