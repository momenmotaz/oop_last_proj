#include "../../include/message.h"
#include "../../include/file_manager.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include <filesystem>

// Helper function to serialize a message to string
std::string serializeMessage(const Message& msg) {
    std::stringstream ss;
    ss << msg.getSenderId() << "|"
       << msg.getReceiverId() << "|"
       << msg.getContent() << "|"
       << msg.getTimestamp().toString() << "|"
       << (msg.isRead() ? "1" : "0");
    return ss.str();
}

// Helper function to deserialize a string to message
Message deserializeMessage(const std::string& str) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> parts;
    
    while (std::getline(ss, item, '|')) {
        parts.push_back(item);
    }
    
    if (parts.size() != 5) {
        throw FacebookException("Invalid message format", "DeserializationError");
    }
    
    int senderId = std::stoi(parts[0]);
    int receiverId = std::stoi(parts[1]);
    std::string content = parts[2];
    
    // Parse timestamp
    std::string timestamp = parts[3];
    std::stringstream ts(timestamp);
    int year, month, day, hour = 0, minute = 0, second = 0;
    char dash, colon;
    
    ts >> year >> dash >> month >> dash >> day;
    if (timestamp.find(':') != std::string::npos) {
        ts >> hour >> colon >> minute >> colon >> second;
    }
    
    Message msg(senderId, receiverId, content);
    if (parts[4] == "1") {
        msg.markAsRead();
    }
    
    return msg;
}

void testMessagePersistence() {
    std::cout << "Testing Message Persistence..." << std::endl;
    
    // Test 1: Save and load a single message
    std::string testFile = "messages_test.txt";
    FileManager& fm = FileManager::getInstance();
    
    Message msg1(1, 2, "Hello, how are you?");
    std::string serialized = serializeMessage(msg1);
    fm.writeFile(testFile, serialized);
    
    std::string loaded = fm.readFile(testFile);
    Message msg2 = deserializeMessage(loaded);
    
    assert(msg2.getSenderId() == msg1.getSenderId() && 
           "Test 1.1 failed: Sender ID mismatch");
    assert(msg2.getReceiverId() == msg1.getReceiverId() && 
           "Test 1.2 failed: Receiver ID mismatch");
    assert(msg2.getContent() == msg1.getContent() && 
           "Test 1.3 failed: Content mismatch");
    std::cout << "Test 1 passed: Single message persistence" << std::endl;
}

void testMultipleMessagesPersistence() {
    std::cout << "\nTesting Multiple Messages Persistence..." << std::endl;
    
    // Test 2: Save and load multiple messages
    std::string testFile = "messages_test.txt";
    FileManager& fm = FileManager::getInstance();
    
    std::vector<Message> messages = {
        Message(1, 2, "First message"),
        Message(2, 1, "Reply to first"),
        Message(1, 2, "Another message")
    };
    
    // Mark some messages as read
    messages[0].markAsRead();
    messages[1].markAsRead();
    
    // Save all messages
    std::stringstream ss;
    for (const auto& msg : messages) {
        ss << serializeMessage(msg) << "\n";
    }
    fm.writeFile(testFile, ss.str());
    
    // Load and verify
    std::vector<std::string> lines = fm.readLines(testFile);
    assert(lines.size() == messages.size() && 
           "Test 2.1 failed: Number of messages mismatch");
    
    for (size_t i = 0; i < lines.size(); ++i) {
        Message loaded = deserializeMessage(lines[i]);
        assert(loaded.getSenderId() == messages[i].getSenderId() && 
               "Test 2.2 failed: Sender ID mismatch");
        assert(loaded.getReceiverId() == messages[i].getReceiverId() && 
               "Test 2.3 failed: Receiver ID mismatch");
        assert(loaded.getContent() == messages[i].getContent() && 
               "Test 2.4 failed: Content mismatch");
        assert(loaded.isRead() == messages[i].isRead() && 
               "Test 2.5 failed: Read status mismatch");
    }
    std::cout << "Test 2 passed: Multiple messages persistence" << std::endl;
}

void testErrorHandling() {
    std::cout << "\nTesting Error Handling..." << std::endl;
    
    // Test 3: Invalid message format
    try {
        deserializeMessage("invalid|format");
        assert(false && "Test 3.1 failed: Should throw exception for invalid format");
    } catch (const FacebookException& e) {
        assert(e.getType() == "DeserializationError" && 
               "Test 3.2 failed: Wrong exception type");
    }
    std::cout << "Test 3 passed: Error handling" << std::endl;
}

void cleanup() {
    // Clean up test files
    std::string testFile = "messages_test.txt";
    if (std::filesystem::exists(testFile)) {
        std::filesystem::remove(testFile);
    }
}

int main() {
    try {
        testMessagePersistence();
        testMultipleMessagesPersistence();
        testErrorHandling();
        cleanup();
        std::cout << "\nAll message persistence integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        cleanup();
        return 1;
    }
}
