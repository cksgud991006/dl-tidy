#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <filesystem>

#include "file_change_handler_interface.h"
#include "task_executor_interface.h"

class SharedDirectoryManager: public IFileChangeHandler, public ITaskExecutor {
    private:
        std::mutex mutex_;
        std::queue<std::filesystem::path> jobQueue_;
        std::condition_variable jobCv_;
        std::atomic<bool> isFinished_ {false};

    public:
        ~SharedDirectoryManager();
        void onFileChange(std::filesystem::path path) override;
        void execute() override;
        void scheduleFilesJob(std::filesystem::path path);
        void executeFilesJob();
        void stop();

};