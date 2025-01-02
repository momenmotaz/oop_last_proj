#include "../../include/conversation.h"
#include "../../include/message.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

void testConversationWithRealMessages() {
    std::cout << "Testing Conversation with Real Messages..." << std::endl;
    
    // Test 1: Create conversation and add real messages
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    auto msg1 = std::make_shared<Message>(1, 2, "Hello, how are you?");
    auto msg2 = std::make_shared<Message>(2, 1, "I'm good, thanks!");
    auto msg3 = std::make_shared<Message>(1, 2, "Great to hear that!");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    conv.addMessage(msg3);
    
    assert(conv.getMessages().size() == 3 && "Test 1.1 failed: Message count mismatch");
    assert(conv.getMessages()[0]->getContent() == "Hello, how are you?" && 
           "Test 1.2 failed: First message content mismatch");
    std::cout << "Test 1 passed: Real messages added successfully" << std::endl;
}

void testMessageTimestamps() {
    std::cout << "\nTesting Message Timestamps in Conversation..." << std::endl;
    
    // Test 2: Verify message timestamps
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    auto msg1 = std::make_shared<Message>(1, 2, "First message");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto msg2 = std::make_shared<Message>(2, 1, "Second message");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    
    assert(msg1->getTimestamp() < msg2->getTimestamp() && 
           "Test 2.1 failed: Messages should be ordered by timestamp");
    std::cout << "Test 2 passed: Message timestamps are correct" << std::endl;
}

void testMessageReadStatus() {
    std::cout << "\nTesting Message Read Status..." << std::endl;
    
    // Test 3: Track read/unread status
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    auto msg1 = std::make_shared<Message>(1, 2, "Message 1");
    auto msg2 = std::make_shared<Message>(1, 2, "Message 2");
    auto msg3 = std::make_shared<Message>(2, 1, "Message 3");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    conv.addMessage(msg3);
    
    auto unreadBefore = conv.getUnreadMessages(2);
    assert(unreadBefore.size() == 2 && "Test 3.1 failed: Initial unread count wrong");
    
    msg1->markAsRead();
    auto unreadAfter = conv.getUnreadMessages(2);
    assert(unreadAfter.size() == 1 && "Test 3.2 failed: Unread count after marking as read");
    std::cout << "Test 3 passed: Message read status tracking works" << std::endl;
}

void testParticipantInteractions() {
    std::cout << "\nTesting Complex Participant Interactions..." << std::endl;
    
    // Test 4: Complex participant scenarios
    std::vector<int> participants = {1, 2};
    Conversation<Message> conv(participants);
    
    // Add a new participant
    conv.addParticipant(3);
    
    // Try sending messages with new participant
    auto msg1 = std::make_shared<Message>(3, 1, "Hello from new participant!");
    conv.addMessage(msg1);
    
    // Remove a participant
    conv.removeParticipant(2);
    
    // Try sending message to removed participant
    try {
        auto msg2 = std::make_shared<Message>(1, 2, "Message to removed participant");
        conv.addMessage(msg2);
        assert(false && "Test 4.1 failed: Should not allow message to removed participant");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 4.2 failed: Wrong exception type");
    }
    
    std::cout << "Test 4 passed: Complex participant interactions handled correctly" << std::endl;
}

void testMessageFiltering() {
    std::cout << "\nTesting Message Filtering..." << std::endl;
    
    // Test 5: Filter messages by various criteria
    std::vector<int> participants = {1, 2, 3};
    Conversation<Message> conv(participants);
    
    // Add messages from different participants
    conv.addMessage(std::make_shared<Message>(1, 2, "Message 1"));
    conv.addMessage(std::make_shared<Message>(2, 1, "Message 2"));
    conv.addMessage(std::make_shared<Message>(1, 3, "Message 3"));
    conv.addMessage(std::make_shared<Message>(3, 1, "Message 4"));
    conv.addMessage(std::make_shared<Message>(2, 3, "Message 5"));
    
    // Test messages by user
    auto user1Messages = conv.getMessagesByUser(1);
    assert(user1Messages.size() == 2 && "Test 5.1 failed: User 1 message count wrong");
    
    auto user2Messages = conv.getMessagesByUser(2);
    assert(user2Messages.size() == 2 && "Test 5.2 failed: User 2 message count wrong");
    
    auto user3Messages = conv.getMessagesByUser(3);
    assert(user3Messages.size() == 1 && "Test 5.3 failed: User 3 message count wrong");
    
    std::cout << "Test 5 passed: Message filtering works correctly" << std::endl;
}

int main() {
    try {
        testConversationWithRealMessages();
        testMessageTimestamps();
        testMessageReadStatus();
        testParticipantInteractions();
        testMessageFiltering();
        
        std::cout << "\nAll conversation integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
