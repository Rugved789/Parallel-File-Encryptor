#include <iostream>
#include <filesystem>
#include <algorithm>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"
#include "./src/app/encryptDecrypt/Cryption.hpp"


namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    if (argc >= 3 && std::string(argv[1]) == "--process-task") {
        std::string taskData = argv[2];
        executeCryption(taskData);
        return 0;
    }

    std::string directory;
    std::string action;

    std::cout << "Enter the directory path: ";
    std::getline(std::cin, directory);

    std::cout << "Enter the action (encryption/decryption) : " << std::endl;
    std::getline(std::cin, action);

    // Convert action to uppercase for comparison
    std::transform(action.begin(), action.end(), action.begin(), ::toupper);

    try{
        if(fs::exists(directory) && fs::is_directory(directory)){
            ProcessManagement processManager;
            int submitted = 0;
            for(const auto &entry : fs::recursive_directory_iterator(directory)){
                if(entry.is_regular_file()){
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = io.getFileStream();
                    if(f_stream.is_open()){
                        Action taskAction = (action.find("ENCRYPT") != std::string::npos ? Action::ENCRYPT : Action::DECRYPT);
                        auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
                        if (processManager.submitToQueue(std::move(task))) {
                            ++submitted;
                        }
                    }
                    else{
                        std::cout << "Unable to open the file : " << filePath << std::endl;
                    }
                }
            }
            std::cout << "Submitted " << submitted << " worker tasks." << std::endl;
        }else{
            std::cout << "Invalid directory path: " << std::endl;
        }
    }catch(const fs::filesystem_error &ex){
        std::cout << "File System error : " << ex.what() << std::endl;
    }
}