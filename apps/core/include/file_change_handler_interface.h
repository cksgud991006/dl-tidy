#pragma once

class IFileChangeHandler {
    public:
        virtual void onFileChange(std::filesystem::path path) = 0;
};