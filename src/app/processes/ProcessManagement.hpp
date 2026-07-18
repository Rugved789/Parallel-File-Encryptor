#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include"Task.hpp"
#include <queue>
#include <memory>
#include <mutex>
#include <atomic>
#include <semaphore.h>

class ProcessManagement{
    public :
        ProcessManagement();
        bool submitToQueue(std::unique_ptr<Task> task);
        void executeTasks();

    private:
        struct SharedMemory{
            std::atomic<int> size;
            char tasks[100][253];
            int front;
            int back;
            void printShareMemory(){
                std::cout<<size<<std::endl;

            }
        };
        SharedMemory* shareMem;
        int shmFd;
        const char* SHM_NAME = "/my_queue";

};

#endif