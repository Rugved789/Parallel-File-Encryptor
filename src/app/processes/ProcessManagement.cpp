#include "ProcessManagement.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include "../encryptDecrypt/Cryption.hpp"
#include <windows.h>
#include <cstdlib>

// Global handles for IPC (Windows) - static to avoid multiple definition
static HANDLE g_itemsSemaphone = nullptr;
static HANDLE g_emptySlotsSemaphone = nullptr;
static HANDLE g_hMapFile = nullptr;

ProcessManagement::ProcessManagement(){
    // Create semaphores for Windows
    g_itemsSemaphone = CreateSemaphoreA(nullptr, 0, 1000, "items_semaphone");
    g_emptySlotsSemaphone = CreateSemaphoreA(nullptr, 1000, 1000, "empty_slots_semaphone");
    
    // Create shared memory file mapping
    g_hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 
                                     0, sizeof(SharedMemory), "my_queue");
    
    // Map into address space
    sharedMem = static_cast<SharedMemory *>(MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemory)));
    
    if (sharedMem) {
        sharedMem->front = 0;
        sharedMem->rear = 0;
        sharedMem->size.store(0);
    }
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task){
    WaitForSingleObject(g_emptySlotsSemaphone, INFINITE);
    std::unique_lock<std::mutex> lock(queueLock);
    if(sharedMem->size.load()>=1000){
        ReleaseSemaphore(g_emptySlotsSemaphone, 1, nullptr);
        return false;
    }

    std::string taskData = task->toString();
    std::strncpy(sharedMem->tasks[sharedMem->rear], taskData.c_str(), sizeof(sharedMem->tasks[sharedMem->rear]) - 1);
    sharedMem->tasks[sharedMem->rear][sizeof(sharedMem->tasks[sharedMem->rear]) - 1] = '\0';
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();
    ReleaseSemaphore(g_itemsSemaphone, 1, nullptr);

    char exePath[MAX_PATH] = {};
    DWORD exeLength = GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    if (exeLength == 0) {
        return false;
    }

    std::string commandLine = std::string("\"") + exePath + "\" --process-task \"" + taskData + "\"";
    std::vector<char> commandBuffer(commandLine.begin(), commandLine.end());
    commandBuffer.push_back('\0');

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    if (CreateProcessA(exePath, commandBuffer.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cout << "Child process created for " << taskData << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }

    return false;
}

void ProcessManagement::executeTasks(){
    WaitForSingleObject(g_itemsSemaphone, INFINITE);
    std::unique_lock<std::mutex> lock(queueLock);
    char taskStr[256];
    strcpy_s(taskStr, 256, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) %1000;
    sharedMem->size.fetch_sub(1);
    lock.unlock();
    ReleaseSemaphore(g_emptySlotsSemaphone, 1, nullptr);
    std::cout<<"Executing task"<<std::endl;
    executeCryption(taskStr);
}

ProcessManagement::~ProcessManagement(){
    if (sharedMem) {
        UnmapViewOfFile(sharedMem);
    }
    if (g_hMapFile) {
        CloseHandle(g_hMapFile);
    }
    if (g_itemsSemaphone) {
        CloseHandle(g_itemsSemaphone);
    }
    if (g_emptySlotsSemaphone) {
        CloseHandle(g_emptySlotsSemaphone);
    }
}