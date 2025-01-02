#include "../include/file_manager.h"
#include <fstream>
#include <sstream>
#include <string>

// Initialize static member
FileManager* FileManager::instance = nullptr;

FileManager& FileManager::getInstance() {
    if (!instance) {
        instance = new FileManager();
    }
    return *instance;
}

void FileManager::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FacebookException("Could not open file for writing: " + filename, "FileError");
    }
    
    file << content;
    if (!file) {
        throw FacebookException("Error writing to file: " + filename, "FileError");
    }
    file.close();
}

std::string FileManager::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FacebookException("Could not open file: " + filename, "FileError");
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return content;
}

bool FileManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

void FileManager::deleteFile(const std::string& filename) {
    if (std::remove(filename.c_str()) != 0) {
        throw FacebookException("Could not delete file: " + filename, "FileError");
    }
}

FileManager::~FileManager() {
    delete instance;
    instance = nullptr;
}
