#include "../../include/file_manager.h"
#include <cassert>
#include <iostream>
#include <filesystem>

void testSingleton() {
    std::cout << "Testing Singleton Pattern..." << std::endl;
    
    // Test 1: Get instance should return the same instance
    FileManager& instance1 = FileManager::getInstance();
    FileManager& instance2 = FileManager::getInstance();
    assert(&instance1 == &instance2 && "Test 1 failed: getInstance should return same instance");
    std::cout << "Test 1 passed: Singleton instance" << std::endl;
}

void testFileOperations() {
    std::cout << "\nTesting File Operations..." << std::endl;
    FileManager& fm = FileManager::getInstance();
    
    // Test 2: Write and read file
    std::string testFile = "test_file.txt";
    std::string content = "Hello, World!\nThis is a test file.";
    
    fm.writeFile(testFile, content);
    assert(fm.fileExists(testFile) && "Test 2.1 failed: File should exist after writing");
    
    std::string readContent = fm.readFile(testFile);
    assert(readContent == content + "\n" && "Test 2.2 failed: Read content should match written content");
    std::cout << "Test 2 passed: Write and read file" << std::endl;
    
    // Test 3: Append to file
    std::string appendContent = "\nAppended content";  
    fm.appendToFile(testFile, appendContent);
    readContent = fm.readFile(testFile);
    assert(readContent.find(appendContent) != std::string::npos && 
           "Test 3 failed: Appended content should be in file");
    std::cout << "Test 3 passed: Append to file" << std::endl;
    
    // Test 4: Read lines
    std::vector<std::string> lines = fm.readLines(testFile);
    assert(lines.size() >= 2 && "Test 4.1 failed: Should have at least 2 lines");
    assert(lines[0] == "Hello, World!" && "Test 4.2 failed: First line mismatch");
    assert(lines[1] == "This is a test file." && "Test 4.3 failed: Second line mismatch");
    std::cout << "Test 4 passed: Read lines" << std::endl;
    
    // Test 5: Delete file
    fm.deleteFile(testFile);
    assert(!fm.fileExists(testFile) && "Test 5 failed: File should not exist after deletion");
    std::cout << "Test 5 passed: Delete file" << std::endl;
}

void testErrorHandling() {
    std::cout << "\nTesting Error Handling..." << std::endl;
    FileManager& fm = FileManager::getInstance();
    
    // Test 6: Reading non-existent file
    try {
        fm.readFile("nonexistent.txt");
        assert(false && "Test 6.1 failed: Should throw exception for non-existent file");
    } catch (const FacebookException& e) {
        assert(e.getType() == "FileError" && "Test 6.2 failed: Wrong exception type");
    }
    std::cout << "Test 6 passed: Error handling for non-existent file" << std::endl;
}

void cleanup() {
    // Clean up any test files that might be left
    std::string testFile = "test_file.txt";
    if (std::filesystem::exists(testFile)) {
        std::filesystem::remove(testFile);
    }
}

int main() {
    try {
        testSingleton();
        testFileOperations();
        testErrorHandling();
        cleanup();
        std::cout << "\nAll FileManager tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        cleanup();
        return 1;
    }
}
