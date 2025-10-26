#include "file_system.h"
#include <shlobj.h>  // For SHGetKnownFolderPath
#include <knownfolders.h>
#include <wchar.h>

std::filesystem::path getDownloadsPath() {

    PWSTR pathTmp = nullptr;
    
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &pathTmp))) {

        std::filesystem::path downloads(pathTmp);

        CoTaskMemFree(pathTmp);

        return downloads;
    }

    // fallback: user profile + "Downloads"
    return std::filesystem::path(std::getenv("USERPROFILE")) / "Downloads";
}
