#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "facebook_exception.h"
#include <string>
#include <vector>
#include <fstream>
#include <memory>

class FileManager {
private:
    // Singleton instance
    static FileManager* instance;
    
    // Private constructor for singleton
    FileManager() = default;
    
    // Delete copy constructor and assignment operator
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

public:
    // Get singleton instance
    static FileManager& getInstance() {
        if (!instance) {
            instance = new FileManager();
        }
        return *instance;
    }
    
    // File operations
    void writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw FacebookException("Could not open file for writing: " + filename, "FileError");
        }
        
        file << content;
        if (!file) {
            throw FacebookException("Error writing to file: " + filename, "FileError");
        }
    }
    
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw FacebookException("Could not open file: " + filename, "FileError");
        }
        
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        
        return content;
    }
    
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
    
    void deleteFile(const std::string& filename) {
        if (std::remove(filename.c_str()) != 0) {
            throw FacebookException("Could not delete file: " + filename, "FileError");
        }
    }
    
    // Destructor
    ~FileManager() {}
};

// Initialize static member
FileManager* FileManager::instance = nullptr;

#endif // FILE_MANAGER_H
