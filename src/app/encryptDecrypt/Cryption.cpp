#include "Cryption.hpp"
#include "../file_handling/ReadEnv.cpp"
#include <vector>
#include <fstream>

const unsigned char ENCRYPTION_MARKER = 0xFF;

int executeCryption(Task &task){
    ReadEnv env;

    std::string envKey = env.getenv();
    int key = std::stoi(envKey);

    // Read entire file into memory
    task.f_stream.seekg(0, std::ios::end);
    std::streamsize fileSize = task.f_stream.tellg();
    task.f_stream.seekg(0, std::ios::beg);
    
    std::vector<unsigned char> fileContent(fileSize);
    task.f_stream.read(reinterpret_cast<char*>(fileContent.data()), fileSize);
    task.f_stream.close();

    // Check if file is already encrypted (first byte is marker 0xFF)
    bool isEncrypted = (fileSize > 0 && fileContent[0] == ENCRYPTION_MARKER);

    // Process the content
    if(task.action == Action::ENCRYPT){
        if(isEncrypted){
            // File is already encrypted, skip encryption
            return 0;
        }
        // Encrypt: add marker + encrypt the actual content
        std::vector<unsigned char> encrypted;
        encrypted.push_back(ENCRYPTION_MARKER);
        
        for(auto &ch : fileContent){
            encrypted.push_back((ch + key) % 256);
        }
        fileContent = std::move(encrypted);
    }else{
        if(!isEncrypted){
            // File is not encrypted, skip decryption
            return 0;
        }
        // Decrypt: remove marker and decrypt the content
        std::vector<unsigned char> decrypted;
        for(size_t i = 1; i < fileContent.size(); i++){
            decrypted.push_back((fileContent[i] - key + 256) % 256);
        }
        fileContent = std::move(decrypted);
    }

    // Write back to file
    std::ofstream out(task.filePath, std::ios::out | std::ios::binary | std::ios::trunc);
    out.write(reinterpret_cast<char*>(fileContent.data()), fileContent.size());
    out.flush();
    out.close();
    
    return 0;
}

// Overload for string input (used by CryptionMain)
int executeCryption(const std::string &taskData){
    Task task = Task::fromString(taskData);
    return executeCryption(task);
}