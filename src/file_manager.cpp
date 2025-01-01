#include "../include/file_manager.h"
#include <fstream>
#include <sstream>

// Initialize the singleton instance
FileManager* FileManager::instance = nullptr;

FileManager& FileManager::getInstance() {
    if (instance == nullptr) {
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
    file.close();
}

std::string FileManager::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FacebookException("Could not open file for reading: " + filename, "FileError");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

bool FileManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
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
