#include "../../include/conversation.h"
#include "../../include/message.h"
#include "../../include/datetime.h"
#include "../../include/facebook_exception.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>

// Helper function to create a formatted string of current time
std::string getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm* ltm = std::localtime(&time);
    
    std::stringstream ss;
    ss << ltm->tm_year + 1900 << "-"
       << ltm->tm_mon + 1 << "-"
       << ltm->tm_mday << " "
       << ltm->tm_hour << ":"
       << ltm->tm_min << ":"
       << ltm->tm_sec;
    return ss.str();
}

void testDateTimeIntegration() {
    std::cout << "Testing DateTime Integration..." << std::endl;
    
    // Test 1: DateTime in Message
    Message msg(1, 2, "Test message");
    const DateTime& msgTime = msg.getTimestamp();
    
    // Verify timestamp is reasonable (not default/invalid)
    assert(msgTime.getYear() >= 2024 && "Test 1.1 failed: Invalid year");
    assert(msgTime.getMonth() >= 1 && msgTime.getMonth() <= 12 && "Test 1.2 failed: Invalid month");
    assert(msgTime.getDay() >= 1 && msgTime.getDay() <= 31 && "Test 1.3 failed: Invalid day");
    
    std::cout << "Test 1 passed: DateTime in Message" << std::endl;
}

void testMessageOrdering() {
    std::cout << "\nTesting Message Ordering..." << std::endl;
    
    // Test 2: Message ordering in Conversation
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    // Add messages with delays to ensure different timestamps
    auto msg1 = std::make_shared<Message>(1, 2, "First message");
    std::cout << "Message 1 sent at: " << getCurrentTimeString() << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto msg2 = std::make_shared<Message>(2, 1, "Second message");
    std::cout << "Message 2 sent at: " << getCurrentTimeString() << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto msg3 = std::make_shared<Message>(1, 2, "Third message");
    std::cout << "Message 3 sent at: " << getCurrentTimeString() << std::endl;
    
    // Add messages in different order
    conv.addMessage(msg2);
    conv.addMessage(msg1);
    conv.addMessage(msg3);
    
    // Verify messages are ordered by timestamp
    auto messages = conv.getMessages();
    assert(messages.size() == 3 && "Test 2.1 failed: Message count mismatch");
    assert(messages[0]->getTimestamp() < messages[1]->getTimestamp() && 
           "Test 2.2 failed: Messages not ordered by timestamp (1st and 2nd)");
    assert(messages[1]->getTimestamp() < messages[2]->getTimestamp() && 
           "Test 2.3 failed: Messages not ordered by timestamp (2nd and 3rd)");
    
    std::cout << "Test 2 passed: Message ordering in Conversation" << std::endl;
}

void testExceptionHandling() {
    std::cout << "\nTesting Exception Handling..." << std::endl;
    
    // Test 3: Exception propagation
    try {
        Message invalidMsg(-1, -2, "");  // Multiple validation errors
        assert(false && "Test 3.1 failed: Should throw exception for invalid message");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 3.2 failed: Wrong exception type");
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    try {
        auto invalidMsg = std::make_shared<Message>(3, 4, "Message from non-participant");
        conv.addMessage(invalidMsg);
        assert(false && "Test 3.3 failed: Should throw exception for non-participant message");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 3.4 failed: Wrong exception type");
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    
    std::cout << "Test 3 passed: Exception handling" << std::endl;
}

void testComplexScenario() {
    std::cout << "\nTesting Complex Scenario..." << std::endl;
    
    // Test 4: Complex interaction scenario
    std::vector<int> participants = {1, 2, 3};
    Conversation<Message> conv(participants);
    
    // Simulate a group chat scenario
    auto msg1 = std::make_shared<Message>(1, 2, "Hello everyone!");
    auto msg2 = std::make_shared<Message>(2, 1, "Hi there!");
    auto msg3 = std::make_shared<Message>(3, 1, "Hello!");
    auto msg4 = std::make_shared<Message>(1, 3, "How are you?");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    conv.addMessage(msg3);
    conv.addMessage(msg4);
    
    // Test message counts
    assert(conv.getMessages().size() == 4 && "Test 4.1 failed: Total message count wrong");
    assert(conv.getMessagesByUser(1).size() == 2 && "Test 4.2 failed: User 1 message count wrong");
    assert(conv.getMessagesByUser(2).size() == 1 && "Test 4.3 failed: User 2 message count wrong");
    assert(conv.getMessagesByUser(3).size() == 1 && "Test 4.4 failed: User 3 message count wrong");
    
    // Test unread messages
    assert(conv.getUnreadMessages(1).size() == 2 && "Test 4.5 failed: User 1 unread count wrong");
    assert(conv.getUnreadMessages(2).size() == 1 && "Test 4.6 failed: User 2 unread count wrong");
    assert(conv.getUnreadMessages(3).size() == 1 && "Test 4.7 failed: User 3 unread count wrong");
    
    // Mark some messages as read
    msg2->markAsRead();
    msg3->markAsRead();
    
    assert(conv.getUnreadMessages(1).size() == 0 && "Test 4.8 failed: User 1 unread count after marking as read");
    
    // Remove a participant
    conv.removeParticipant(2);
    
    try {
        auto invalidMsg = std::make_shared<Message>(1, 2, "Message to removed participant");
        conv.addMessage(invalidMsg);
        assert(false && "Test 4.9 failed: Should not allow message to removed participant");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 4.10 failed: Wrong exception type");
    }
    
    std::cout << "Test 4 passed: Complex scenario" << std::endl;
}

int main() {
    try {
        testDateTimeIntegration();
        testMessageOrdering();
        testExceptionHandling();
        testComplexScenario();
        
        std::cout << "\nAll system integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
