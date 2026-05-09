#include "shared_directory_manager.h"
#include "log.h"

SharedDirectoryManager::~SharedDirectoryManager() {
    stop();
}

void SharedDirectoryManager::scheduleFilesJob(std::filesystem::path path) {

    {
        std::unique_lock<std::mutex> lock(mutex_);

        for (const auto& entry: std::filesystem::directory_iterator(path)) {
            jobQueue_.push(entry.path());
        }
    }

    jobCv_.notify_all();
}

void SharedDirectoryManager::executeFilesJob() {

    std::filesystem::path fp;

    log_debug("Shared directory manager thread started");

    while (true) 
    {
        // manage shared resource
        {
            std::unique_lock<std::mutex> lock(mutex_);

            jobCv_.wait(lock, [this] { return !jobQueue_.empty() || isFinished_; });

            if (jobQueue_.empty() && isFinished_) break;

            fp = jobQueue_.front();
            jobQueue_.pop();
        }


        // execute task
        try {
            if (std::filesystem::is_directory(fp)) {
                std::filesystem::remove_all(fp);
            } else {
                std::filesystem::remove(fp);
            }
        } catch (const std::filesystem::filesystem_error& e) {
            log_error("Failed to delete: " + fp.string() + " Error: " + e.what());
        }
    }

}

void SharedDirectoryManager::stop() {

    log_debug("Stopping shared directory manager...");

    {
        std::unique_lock<std::mutex> lock(mutex_);
        jobQueue_ = std::queue<std::filesystem::path>(); // clear pending jobs
        isFinished_ = true;
    }

    jobCv_.notify_all();

    log_debug("Shared directory manager stopped");

}

/*
    IFileChangeHandler Implementation
*/

void SharedDirectoryManager::onFileChange(std::filesystem::path path) {
    
    scheduleFilesJob(path);

}

/*
    ITaskExecutor Implementation
*/
void SharedDirectoryManager::execute() {

    executeFilesJob();

}
