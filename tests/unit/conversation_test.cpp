#include "../../include/conversation.h"
#include <cassert>
#include <iostream>

// Mock Message class for testing
class MockMessage {
public:
    int senderId;
    int receiverId;
    std::string content;
    bool read;

    MockMessage(int sender, int receiver, const std::string& msg)
        : senderId(sender), receiverId(receiver), content(msg), read(false) {}
    
    int getSenderId() const { return senderId; }
    int getReceiverId() const { return receiverId; }
    bool isRead() const { return read; }
    void markAsRead() { read = true; }
};

void testConversationCreation() {
    std::cout << "Testing Conversation Creation..." << std::endl;
    
    // Test 1: Create valid conversation
    std::vector<int> participants = {1, 2};
    Conversation<MockMessage> conv1(participants);
    assert(conv1.getParticipants() == participants && "Test 1.1 failed: Participants mismatch");
    assert(conv1.getMessages().empty() && "Test 1.2 failed: New conversation should have no messages");
    std::cout << "Test 1 passed: Valid conversation creation" << std::endl;
    
    // Test 2: Create conversation with invalid participants
    try {
        std::vector<int> invalidParticipants = {0, -1};
        Conversation<MockMessage> conv2(invalidParticipants);
        assert(false && "Test 2.1 failed: Should throw exception for invalid participants");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.2 failed: Wrong exception type");
    }
    std::cout << "Test 2 passed: Invalid conversation creation" << std::endl;
    
    // Test 3: Create conversation with duplicate participants
    try {
        std::vector<int> duplicateParticipants = {1, 1};
        Conversation<MockMessage> conv3(duplicateParticipants);
        assert(false && "Test 3.1 failed: Should throw exception for duplicate participants");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 3.2 failed: Wrong exception type");
    }
    std::cout << "Test 3 passed: Duplicate participants handling" << std::endl;
}

void testMessageManagement() {
    std::cout << "\nTesting Message Management..." << std::endl;
    
    std::vector<int> participants = {1, 2};
    Conversation<MockMessage> conv(participants);
    
    // Test 4: Add messages
    auto msg1 = std::make_shared<MockMessage>(1, 2, "Hello!");
    auto msg2 = std::make_shared<MockMessage>(2, 1, "Hi there!");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    
    assert(conv.getMessages().size() == 2 && "Test 4.1 failed: Message count mismatch");
    std::cout << "Test 4 passed: Add messages" << std::endl;
    
    // Test 5: Invalid message (from non-participant)
    try {
        auto invalidMsg = std::make_shared<MockMessage>(3, 1, "Invalid!");
        conv.addMessage(invalidMsg);
        assert(false && "Test 5.1 failed: Should throw exception for invalid sender");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 5.2 failed: Wrong exception type");
    }
    std::cout << "Test 5 passed: Invalid message handling" << std::endl;
}

void testMessageRetrieval() {
    std::cout << "\nTesting Message Retrieval..." << std::endl;
    
    std::vector<int> participants = {1, 2};
    Conversation<MockMessage> conv(participants);
    
    // Test 6: Get messages by user
    auto msg1 = std::make_shared<MockMessage>(1, 2, "Message 1");
    auto msg2 = std::make_shared<MockMessage>(2, 1, "Message 2");
    auto msg3 = std::make_shared<MockMessage>(1, 2, "Message 3");
    
    conv.addMessage(msg1);
    conv.addMessage(msg2);
    conv.addMessage(msg3);
    
    auto user1Messages = conv.getMessagesByUser(1);
    assert(user1Messages.size() == 2 && "Test 6.1 failed: User 1 message count mismatch");
    
    auto user2Messages = conv.getMessagesByUser(2);
    assert(user2Messages.size() == 1 && "Test 6.2 failed: User 2 message count mismatch");
    std::cout << "Test 6 passed: Get messages by user" << std::endl;
    
    // Test 7: Get unread messages
    auto unreadMessages = conv.getUnreadMessages(2);
    assert(unreadMessages.size() == 2 && "Test 7.1 failed: Unread message count mismatch");
    
    msg1->markAsRead();
    unreadMessages = conv.getUnreadMessages(2);
    assert(unreadMessages.size() == 1 && "Test 7.2 failed: Unread message count after marking as read");
    std::cout << "Test 7 passed: Get unread messages" << std::endl;
}

void testParticipantManagement() {
    std::cout << "\nTesting Participant Management..." << std::endl;
    
    std::vector<int> participants = {1, 2};
    Conversation<MockMessage> conv(participants);
    
    // Test 8: Add participant
    conv.addParticipant(3);
    assert(conv.getParticipants().size() == 3 && "Test 8.1 failed: Participant count mismatch");
    assert(conv.isParticipant(3) && "Test 8.2 failed: New participant not found");
    std::cout << "Test 8 passed: Add participant" << std::endl;
    
    // Test 9: Remove participant
    conv.removeParticipant(2);
    assert(conv.getParticipants().size() == 2 && "Test 9.1 failed: Participant count after removal");
    assert(!conv.isParticipant(2) && "Test 9.2 failed: Removed participant still exists");
    std::cout << "Test 9 passed: Remove participant" << std::endl;
    
    // Test 10: Invalid participant operations
    try {
        conv.addParticipant(0);  // Invalid user ID
        assert(false && "Test 10.1 failed: Should throw exception for invalid participant");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 10.2 failed: Wrong exception type");
    }
    std::cout << "Test 10 passed: Invalid participant operations" << std::endl;
}

int main() {
    try {
        testConversationCreation();
        testMessageManagement();
        testMessageRetrieval();
        testParticipantManagement();
        
        std::cout << "\nAll conversation tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
