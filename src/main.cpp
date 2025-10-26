#include <iostream>
#include <string>
#include "file_system.h"
#include "watch.h"
#include "log.h"
#include <format>

int main() {

    auto const path = getDownloadsPath();

    log(std::format("Retrieved {}", path.string()));

    watch(path);

    return 0;
}
