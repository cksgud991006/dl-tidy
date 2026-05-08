#include <shlobj.h>  // For SHGetKnownFolderPath
#include <knownfolders.h>
#include <wchar.h>
#include "directory.h"

std::filesystem::path getDownloadsPath() {

    PWSTR pathTmp = nullptr; // Warning: Needs to be freed
    
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &pathTmp))) {

        std::filesystem::path downloads(pathTmp);

        CoTaskMemFree(pathTmp);

        return downloads;
    }

    // fallback: user profile + "Downloads"
    return std::filesystem::path(std::getenv("USERPROFILE")) / "Downloads";
}

std::filesystem::path getLocalAppDataPath() {

    PWSTR pathTmp = nullptr;
    
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pathTmp))) {

        std::filesystem::path localAppData(pathTmp);

        CoTaskMemFree(pathTmp);

        return localAppData;
    }

    throw std::runtime_error("Failed to get LocalAppData path");
}

std::filesystem::path getLogPath() {

    PWSTR pathTmp = nullptr;
    
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pathTmp))) {

        std::filesystem::path localLogPath(pathTmp);
    
        localLogPath /= "DL-Tidy";
        localLogPath /= "logs";

        CoTaskMemFree(pathTmp);

        return localLogPath;
    }

    throw std::runtime_error("Failed to get LocalAppData path");
}