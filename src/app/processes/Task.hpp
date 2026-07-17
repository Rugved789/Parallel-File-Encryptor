#ifndef TASK_HPP
#define TASK_HPP
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <stdexcept>
#include "../file_handling/IO.hpp"

enum class Action{
    ENCRYPT,
    DECRYPT
};

struct Task{
    std::string filePath;
    std::fstream f_stream;
    Action action;

    Task(std::fstream &&stream, Action act, std::string path)
        : filePath(std::move(path)), f_stream(std::move(stream)), action(act) {}

    std::string toString() const {
        std::ostringstream oss;
        oss << filePath << "," << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
        return oss.str();
    }

    static Task fromString(const std::string &taskData){
        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;

        if(std::getline(iss, filePath, ',') && std::getline(iss, actionStr)){
            Action action = (actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);
            IO io(filePath);
            std::fstream fileStream = io.getFileStream();
            if(fileStream.is_open()){
                return Task(std::move(fileStream), action, filePath);
            }else{
                throw std::runtime_error("Failed to open file : " + filePath);
            }
        }else{
            throw std::runtime_error("Invalid task data format");
        }
    }
};

#endif