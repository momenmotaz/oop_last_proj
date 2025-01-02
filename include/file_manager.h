#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "facebook_exception.h"
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <filesystem>
#include <sstream>

class FileManager {
private:
    // Singleton instance
    static FileManager* instance;
    
    // File paths
    std::string usersFile;
    std::string postsFile;
    std::string conversationsFile;
    
    // Private constructor for singleton
    FileManager() : 
        usersFile("data/users.json"),
        postsFile("data/posts.json"),
        conversationsFile("data/conversations.json") {
        // Create data directory if it doesn't exist
        std::filesystem::create_directories("data");
    }
    
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
    
    // File paths getters
    const std::string& getUsersFile() const { return usersFile; }
    const std::string& getPostsFile() const { return postsFile; }
    const std::string& getConversationsFile() const { return conversationsFile; }
    
    // Data operations
    template<typename T>
    void saveData(const std::string& filename, const T& data) {
        try {
            writeFile(filename, data.serialize());
        } catch (const std::exception& e) {
            throw FacebookException("Failed to save data: " + std::string(e.what()), "FileError");
        }
    }
    
    template<typename T>
    T loadData(const std::string& filename) {
        try {
            if (!fileExists(filename)) {
                return T();  // Return empty object if file doesn't exist
            }
            std::string content = readFile(filename);
            return T::deserialize(content);
        } catch (const std::exception& e) {
            throw FacebookException("Failed to load data: " + std::string(e.what()), "FileError");
        }
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
    
    void appendToFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            throw FacebookException("Could not open file for appending: " + filename, "FileError");
        }
        
        file << content;
        if (!file) {
            throw FacebookException("Error appending to file: " + filename, "FileError");
        }
    }
    
    std::vector<std::string> readLines(const std::string& filename) {
        std::vector<std::string> lines;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw FacebookException("Could not open file: " + filename, "FileError");
        }
        
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        
        return lines;
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
    
    bool fileExists(const std::string& filename) const {
        std::ifstream file(filename);
        return file.good();
    }
    
    void deleteFile(const std::string& filename) {
        if (std::remove(filename.c_str()) != 0) {
            throw FacebookException("Could not delete file: " + filename, "FileError");
        }
    }
    
    // Handle system initialization
    void initializeSystem() {
        try {
            // Create data files if they don't exist
            if (!fileExists(usersFile)) {
                writeFile(usersFile, "[]");
            }
            if (!fileExists(postsFile)) {
                writeFile(postsFile, "[]");
            }
            if (!fileExists(conversationsFile)) {
                writeFile(conversationsFile, "[]");
            }
        } catch (const std::exception& e) {
            throw FacebookException("Failed to initialize system: " + std::string(e.what()), "SystemError");
        }
    }
    
    // Destructor
    ~FileManager() {
        delete instance;
        instance = nullptr;
    }
};

// Initialize static member
FileManager* FileManager::instance = nullptr;

#endif // FILE_MANAGER_H
