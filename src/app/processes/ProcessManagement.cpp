#include "ProcessManagement.hpp"
#include <iostream>
#include <cstring>
#include "../encryptDecrypt/Cryption.hpp"
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <atomic>
#include <semaphore.h>
ProcessManagement::ProcessManagement(){
    shmFd = shm_open(SHM_NAME,O_CREAT | O_RDWR,0666);
    ftruncate(shmFd,sizeof(SharedMemory));
    SharedMemory = static_cast<SharedMemory *>(mmap(nullptr,sizeof(SharedMemory),PROT_READ | PROT_WRITE | MAP_SHARED,shmFd,0));
    SharedMemory->front = 0;
    SharedMemory->rear = 0;
    SharedMemory->size.store(d:0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task){
    taskQueue.push(std::move(task));
    int pid = fork();
    if(pid<0){
        return false;
    }
    else if (pid>0){
        std::cout<<"Entering the parent process"<<std::endl;
    }
    else{
        std::cout<<"Entering the child process"<<std::endl;
        executeTasks();
        std::cout<<"Exiting child process"<<std::endl;
    }
   return true;
}

void ProcessManagement::executeTasks(){
    while(!taskQueue.empty()){
        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();
        std::cout << "Executing task : " << taskToExecute->toString() << std::endl;
        executeCryption(*taskToExecute);
    }
}