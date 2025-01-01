#include "../../include/message.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

void testMessageCreation() {
    std::cout << "Testing Message Creation..." << std::endl;
    
    // Test 1: Basic message creation
    Message msg1(1, 2, "Hello, how are you?");
    assert(msg1.getSenderId() == 1 && "Test 1.1 failed: Sender ID mismatch");
    assert(msg1.getReceiverId() == 2 && "Test 1.2 failed: Receiver ID mismatch");
    assert(msg1.getContent() == "Hello, how are you?" && "Test 1.3 failed: Content mismatch");
    assert(!msg1.isRead() && "Test 1.4 failed: New message should be unread");
    std::cout << "Test 1 passed: Basic message creation" << std::endl;
}

void testMessageStatus() {
    std::cout << "\nTesting Message Status..." << std::endl;
    
    // Test 2: Message read status
    Message msg(1, 2, "Test message");
    assert(!msg.isRead() && "Test 2.1 failed: New message should be unread");
    msg.markAsRead();
    assert(msg.isRead() && "Test 2.2 failed: Message should be marked as read");
    std::cout << "Test 2 passed: Message status" << std::endl;
}

void testMessageComparison() {
    std::cout << "\nTesting Message Comparison..." << std::endl;
    
    // Test 3: Message comparison (based on timestamp)
    Message msg1(1, 2, "First message");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    Message msg2(1, 2, "Second message");
    
    assert(msg1 < msg2 && "Test 3.1 failed: Earlier message should be less than later message");
    assert(!(msg1 > msg2) && "Test 3.2 failed: Earlier message should not be greater than later message");
    std::cout << "Test 3 passed: Message comparison" << std::endl;
}

void testMessageFormatting() {
    std::cout << "\nTesting Message Formatting..." << std::endl;
    
    // Test 4: Message formatting
    Message msg(1, 2, "Test message");
    std::string formatted = msg.toString();
    assert(formatted.find("From: 1") != std::string::npos && "Test 4.1 failed: Missing sender info");
    assert(formatted.find("To: 2") != std::string::npos && "Test 4.2 failed: Missing receiver info");
    assert(formatted.find("Test message") != std::string::npos && "Test 4.3 failed: Missing content");
    std::cout << "Test 4 passed: Message formatting" << std::endl;
}

void testMessageValidation() {
    std::cout << "\nTesting Message Validation..." << std::endl;
    
    // Test 5: Empty content validation
    try {
        Message msg(1, 2, "");
        assert(false && "Test 5.1 failed: Should not allow empty message");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 5.2 failed: Wrong exception type");
    }
    
    // Test 6: Invalid IDs validation
    try {
        Message msg(-1, 2, "Test");
        assert(false && "Test 6.1 failed: Should not allow negative sender ID");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 6.2 failed: Wrong exception type");
    }
    std::cout << "Tests 5-6 passed: Message validation" << std::endl;
}

int main() {
    try {
        testMessageCreation();
        testMessageStatus();
        testMessageComparison();
        testMessageFormatting();
        testMessageValidation();
        std::cout << "\nAll Message unit tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
