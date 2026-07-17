#include <iostream>
#include <filesystem>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"


namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    std::string directory;
    std::string action;

    std::cout << "Enter the directory path: " << std::endl;
    std::getline(std::cin, directory);

    std::cout << "Enter the action (encryption/decryption) : " << std::endl;
    std::getline(std::cin, action);

    try{
        if(fs::exists(directory) && fs::is_directory(directory)){
            ProcessManagement processManager;
            for(const auto &entry : fs::recursive_directory_iterator(directory)){
                if(entry.is_regular_file()){
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = io.getFileStream();
                    if(f_stream.is_open()){
                        Action taskAction = (action == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);
                        auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
                        processManager.submitToQueue(std::move(task));
                    }
                    else{
                        std::cout << "Unable to open the file : " << filePath << std::endl;
                    }
                }
            }
            processManager.executeTasks();
        }else{
            std::cout << "Invalid directory path: " << std::endl;
        }
    }catch(const fs::filesystem_error &ex){
        std::cout << "File System error : " << ex.what() << std::endl;
    }
}