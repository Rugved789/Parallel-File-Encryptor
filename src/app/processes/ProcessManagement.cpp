#include "ProcessManagement.hpp"
#include <iostream>
#include <utility>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement() = default;

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task){
    if (!task) {
        return false;
    }

    std::thread workerThread(&ProcessManagement::executeTask, this, std::move(task));
    workerThread.detach();
    return true;
}

void ProcessManagement::executeTask(std::unique_ptr<Task> task){
    if (!task) {
        return;
    }

    std::cout << "Executing task for " << task->filePath << std::endl;
    executeCryption(*task);
}

void ProcessManagement::executeTasks(){
    std::cout << "Tasks are executed immediately in their own threads." << std::endl;
}

ProcessManagement::~ProcessManagement() = default;