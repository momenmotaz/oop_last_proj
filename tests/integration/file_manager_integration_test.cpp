#include "../../include/file_manager.h"
#include "../../include/facebook_exception.h"
#include <cassert>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

// Mock data structure for testing
struct MockData {
    std::string content;
    
    static std::string serialize() {
        return R"({"data": "test"})";
    }
    
    static MockData deserialize(const std::string& json) {
        return MockData();
    }
};

void testSystemInitialization() {
    std::cout << "Testing System Initialization..." << std::endl;
    
    FileManager& fm = FileManager::getInstance();
    fm.initializeSystem();
    
    // Test 1: Check if data directory exists
    assert(std::filesystem::exists("data") && "Test 1.1 failed: Data directory should exist");
    assert(std::filesystem::is_directory("data") && "Test 1.2 failed: Data should be a directory");
    
    // Test 2: Check if system files exist
    assert(fm.fileExists(fm.getUsersFile()) && "Test 2.1 failed: Users file should exist");
    assert(fm.fileExists(fm.getPostsFile()) && "Test 2.2 failed: Posts file should exist");
    assert(fm.fileExists(fm.getConversationsFile()) && "Test 2.3 failed: Conversations file should exist");
    
    std::cout << "System initialization tests passed!" << std::endl;
}

void testDataOperations() {
    std::cout << "\nTesting Data Operations..." << std::endl;
    
    FileManager& fm = FileManager::getInstance();
    std::string testFile = "data/test_data.json";
    
    // Test 3: Save and load data
    try {
        fm.saveData(testFile, MockData());
        MockData data = fm.loadData<MockData>(testFile);
        std::cout << "Test 3 passed: Data operations" << std::endl;
    } catch (const FacebookException& e) {
        std::cerr << "Test 3 failed: " << e.what() << std::endl;
        throw;
    }
    
    // Cleanup
    if (fm.fileExists(testFile)) {
        fm.deleteFile(testFile);
    }
}

void testFileConsistency() {
    std::cout << "\nTesting File Consistency..." << std::endl;
    
    FileManager& fm = FileManager::getInstance();
    std::string testFile = "data/test.txt";
    
    // Test 4: Write and verify content
    std::string testContent = "Test content";
    fm.writeFile(testFile, testContent);
    std::string readContent = fm.readFile(testFile);
    assert(readContent == testContent + "\n" && "Test 4.1 failed: Content mismatch");
    
    // Test 5: Append and verify
    std::string appendContent = "\nAppended content";
    fm.appendToFile(testFile, appendContent);
    readContent = fm.readFile(testFile);
    assert(readContent.find(appendContent) != std::string::npos && 
           "Test 5.1 failed: Appended content not found");
    
    // Test 6: Read lines
    std::vector<std::string> lines = fm.readLines(testFile);
    assert(lines.size() >= 2 && "Test 6.1 failed: Should have at least 2 lines");
    assert(lines[0] == "Test content" && "Test 6.2 failed: First line mismatch");
    
    // Cleanup
    fm.deleteFile(testFile);
    
    std::cout << "File consistency tests passed!" << std::endl;
}

void testErrorHandling() {
    std::cout << "\nTesting Error Handling..." << std::endl;
    
    FileManager& fm = FileManager::getInstance();
    
    // Test 7: Reading non-existent file
    try {
        fm.readFile("nonexistent.txt");
        assert(false && "Test 7.1 failed: Should throw exception for non-existent file");
    } catch (const FacebookException& e) {
        assert(e.getType() == "FileError" && "Test 7.2 failed: Wrong exception type");
        std::cout << "Test 7 passed: Error handling for non-existent file" << std::endl;
    }
}

void cleanup() {
    // Clean up test files
    std::string testFile = "data/test.txt";
    std::string testDataFile = "data/test_data.json";
    
    if (std::filesystem::exists(testFile)) {
        std::filesystem::remove(testFile);
    }
    if (std::filesystem::exists(testDataFile)) {
        std::filesystem::remove(testDataFile);
    }
}

int main() {
    try {
        testSystemInitialization();
        testDataOperations();
        testFileConsistency();
        testErrorHandling();
        cleanup();
        std::cout << "\nAll file manager integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        cleanup();
        return 1;
    }
}
