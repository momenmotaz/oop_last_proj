#include "../../include/message.h"
#include "../../include/conversation.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

void testMessageCreation() {
    std::cout << "Testing Message Creation..." << std::endl;
    
    // Test 1: Create valid message
    Message msg1(1, 2, "Hello!");
    assert(msg1.getSenderId() == 1 && "Test 1.1 failed: Sender ID mismatch");
    assert(msg1.getReceiverId() == 2 && "Test 1.2 failed: Receiver ID mismatch");
    assert(msg1.getContent() == "Hello!" && "Test 1.3 failed: Content mismatch");
    assert(!msg1.isRead() && "Test 1.4 failed: New message should be unread");
    std::cout << "Test 1 passed: Valid message creation" << std::endl;
    
    // Test 2: Create message with invalid parameters
    try {
        Message msg2(-1, 2, "Invalid sender");
        assert(false && "Test 2.1 failed: Should throw exception for invalid sender");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.2 failed: Wrong exception type");
    }
    
    try {
        Message msg3(1, -2, "Invalid receiver");
        assert(false && "Test 2.3 failed: Should throw exception for invalid receiver");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.4 failed: Wrong exception type");
    }
    
    try {
        Message msg4(1, 2, "");
        assert(false && "Test 2.5 failed: Should throw exception for empty content");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.6 failed: Wrong exception type");
    }
    std::cout << "Test 2 passed: Invalid message creation" << std::endl;
}

void testMessageTimestamp() {
    std::cout << "\nTesting Message Timestamp..." << std::endl;
    
    // Test 3: Message timestamps
    Message msg1(1, 2, "First message");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    Message msg2(2, 1, "Second message");
    
    assert(msg1 < msg2 && "Test 3.1 failed: First message should have earlier timestamp");
    assert(msg2 > msg1 && "Test 3.2 failed: Second message should have later timestamp");
    assert(!(msg1 == msg2) && "Test 3.3 failed: Messages should not be equal");
    std::cout << "Test 3 passed: Message timestamps" << std::endl;
}

void testMessageInConversation() {
    std::cout << "\nTesting Message in Conversation..." << std::endl;
    
    // Test 4: Add messages to conversation
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    auto msg1 = std::make_shared<Message>(1, 2, "Hello!");
    auto msg2 = std::make_shared<Message>(2, 1, "Hi there!");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    
    // Test message retrieval
    auto messages = conv.getMessages();
    assert(messages.size() == 2 && "Test 4.1 failed: Message count mismatch");
    assert(messages[0]->getContent() == "Hello!" && "Test 4.2 failed: First message content mismatch");
    assert(messages[1]->getContent() == "Hi there!" && "Test 4.3 failed: Second message content mismatch");
    std::cout << "Test 4 passed: Messages in conversation" << std::endl;
    
    // Test 5: Message read status in conversation
    auto unreadBefore = conv.getUnreadMessages(2);
    assert(unreadBefore.size() == 1 && "Test 5.1 failed: Initial unread count wrong");
    
    msg1->markAsRead();
    auto unreadAfter = conv.getUnreadMessages(2);
    assert(unreadAfter.empty() && "Test 5.2 failed: Unread count after marking as read");
    std::cout << "Test 5 passed: Message read status in conversation" << std::endl;
    
    // Test 6: Message filtering in conversation
    auto user1Messages = conv.getMessagesByUser(1);
    assert(user1Messages.size() == 1 && "Test 6.1 failed: User 1 message count wrong");
    assert(user1Messages[0]->getContent() == "Hello!" && "Test 6.2 failed: User 1 message content wrong");
    
    auto user2Messages = conv.getMessagesByUser(2);
    assert(user2Messages.size() == 1 && "Test 6.3 failed: User 2 message count wrong");
    assert(user2Messages[0]->getContent() == "Hi there!" && "Test 6.4 failed: User 2 message content wrong");
    std::cout << "Test 6 passed: Message filtering in conversation" << std::endl;
}

void testMessageToString() {
    std::cout << "\nTesting Message String Representation..." << std::endl;
    
    // Test 7: Message string representation
    Message msg(1, 2, "Test message");
    std::string str = msg.toString();
    
    assert(str.find("From: 1") != std::string::npos && "Test 7.1 failed: Sender not in string");
    assert(str.find("To: 2") != std::string::npos && "Test 7.2 failed: Receiver not in string");
    assert(str.find("Test message") != std::string::npos && "Test 7.3 failed: Content not in string");
    assert(str.find("Unread") != std::string::npos && "Test 7.4 failed: Read status not in string");
    
    msg.markAsRead();
    str = msg.toString();
    assert(str.find("Read") != std::string::npos && "Test 7.5 failed: Updated read status not in string");
    std::cout << "Test 7 passed: Message string representation" << std::endl;
}

int main() {
    try {
        testMessageCreation();
        testMessageTimestamp();
        testMessageInConversation();
        testMessageToString();
        
        std::cout << "\nAll message tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
