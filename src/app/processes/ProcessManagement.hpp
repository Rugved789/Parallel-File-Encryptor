#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include"Task.hpp"
#include <memory>
#include <mutex>
#include <thread>

class ProcessManagement{
    public :
        ProcessManagement();
        ~ProcessManagement();
        bool submitToQueue(std::unique_ptr<Task> task);
        void executeTasks();

    private:
        void executeTask(std::unique_ptr<Task> task);
};

#endif