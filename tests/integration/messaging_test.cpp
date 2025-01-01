#include "../../include/message.h"
#include "../../include/datetime.h"
#include "../../include/facebook_exception.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

void testMessageCreationWithDateTime() {
    std::cout << "Testing Message Creation with DateTime..." << std::endl;
    
    // Test 1: Message creation with current time
    Message msg1(1, 2, "Hello");
    const DateTime& timestamp = msg1.getTimestamp();
    assert(timestamp.isValid() && "Test 1.1 failed: Message timestamp should be valid");
    std::cout << "Test 1 passed: Message creation with timestamp" << std::endl;
}

void testMessageSorting() {
    std::cout << "\nTesting Message Sorting..." << std::endl;
    
    // Test 2: Sort messages by timestamp
    std::vector<Message> messages;
    messages.push_back(Message(1, 2, "First message"));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    messages.push_back(Message(1, 2, "Second message"));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    messages.push_back(Message(1, 2, "Third message"));
    
    // Sort messages
    std::sort(messages.begin(), messages.end());
    
    // Verify order
    for (size_t i = 1; i < messages.size(); ++i) {
        assert(messages[i-1] < messages[i] && 
               "Test 2 failed: Messages not properly sorted by timestamp");
    }
    std::cout << "Test 2 passed: Message sorting" << std::endl;
}

void testExceptionHandling() {
    std::cout << "\nTesting Exception Handling..." << std::endl;
    
    // Test 3: Exception handling across components
    try {
        Message msg(1, 2, "");  // Empty message should throw exception
        assert(false && "Test 3.1 failed: Should throw exception for empty message");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && 
               "Test 3.2 failed: Wrong exception type");
        assert(std::string(e.what()).find("ValidationError:") != std::string::npos && 
               "Test 3.3 failed: Exception message format incorrect");
    }
    std::cout << "Test 3 passed: Exception handling" << std::endl;
}

void testMessageTimestampFormatting() {
    std::cout << "\nTesting Message Timestamp Formatting..." << std::endl;
    
    // Test 4: Message timestamp formatting
    Message msg(1, 2, "Test message");
    std::string formatted = msg.toString();
    assert(formatted.find("Time:") != std::string::npos && 
           "Test 4.1 failed: Missing timestamp in message format");
    
    const DateTime& timestamp = msg.getTimestamp();
    assert(formatted.find(timestamp.toString()) != std::string::npos && 
           "Test 4.2 failed: Timestamp format mismatch");
    std::cout << "Test 4 passed: Timestamp formatting" << std::endl;
}

int main() {
    try {
        testMessageCreationWithDateTime();
        testMessageSorting();
        testExceptionHandling();
        testMessageTimestampFormatting();
        std::cout << "\nAll integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
