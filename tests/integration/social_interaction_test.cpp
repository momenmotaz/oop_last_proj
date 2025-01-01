#include "../../include/message.h"
#include "../../include/comment.h"
#include "../../include/reply.h"
#include "../../include/file_manager.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

// Helper function to serialize social interactions
std::string serializeSocialInteraction(const Message& msg, const Comment& comment, const Reply& reply) {
    std::stringstream ss;
    
    // Serialize message
    ss << "MESSAGE\n"
       << msg.getSenderId() << "|"
       << msg.getReceiverId() << "|"
       << msg.getContent() << "|"
       << msg.getTimestamp().toString() << "|"
       << (msg.isRead() ? "1" : "0") << "\n";
       
    // Serialize comment
    ss << "COMMENT\n"
       << comment.getId() << "|"
       << comment.getAuthorId() << "|"
       << comment.getContent() << "|"
       << comment.getTimestamp().toString() << "|";
    
    // Add likers
    for (const auto& likerId : comment.getLikerIds()) {
        ss << likerId << ",";
    }
    ss << "\n";
    
    // Serialize reply
    ss << "REPLY\n"
       << reply.getId() << "|"
       << reply.getAuthorId() << "|"
       << reply.getCommentId() << "|"
       << reply.getContent() << "|"
       << reply.getTimestamp().toString() << "|";
    
    // Add likers
    for (const auto& likerId : reply.getLikerIds()) {
        ss << likerId << ",";
    }
    
    return ss.str();
}

void testSocialInteractionFlow() {
    std::cout << "Testing Social Interaction Flow..." << std::endl;
    
    FileManager& fm = FileManager::getInstance();
    std::string testFile = "social_test.txt";
    
    try {
        // Test 1: Create a message, comment, and reply
        Message msg(1, 2, "Hey, check out this post!");
        Comment comment(2, "Thanks for sharing!");
        Reply reply(3, comment.getId(), "You're welcome!");
        
        std::cout << "Test 1 passed: Created social interactions" << std::endl;
        
        // Test 2: Add likes
        comment.addLike(1);  // Original sender likes the comment
        comment.addLike(3);  // Reply author likes the comment
        reply.addLike(2);    // Comment author likes the reply
        
        assert(comment.getLikesCount() == 2 && "Test 2.1 failed: Comment likes count mismatch");
        assert(reply.getLikesCount() == 1 && "Test 2.2 failed: Reply likes count mismatch");
        std::cout << "Test 2 passed: Like management" << std::endl;
        
        // Test 3: Save to file
        std::string serialized = serializeSocialInteraction(msg, comment, reply);
        fm.writeFile(testFile, serialized);
        
        std::string loaded = fm.readFile(testFile);
        assert(!loaded.empty() && "Test 3.1 failed: File content is empty");
        assert(loaded.find("MESSAGE") != std::string::npos && "Test 3.2 failed: Message not found");
        assert(loaded.find("COMMENT") != std::string::npos && "Test 3.3 failed: Comment not found");
        assert(loaded.find("REPLY") != std::string::npos && "Test 3.4 failed: Reply not found");
        std::cout << "Test 3 passed: File persistence" << std::endl;
        
        // Test 4: Verify timestamps exist
        assert(!msg.getTimestamp().toString().empty() && "Test 4.1 failed: Message timestamp is empty");
        assert(!comment.getTimestamp().toString().empty() && "Test 4.2 failed: Comment timestamp is empty");
        assert(!reply.getTimestamp().toString().empty() && "Test 4.3 failed: Reply timestamp is empty");
        std::cout << "Test 4 passed: Timestamp existence" << std::endl;
        
        // Test 5: Exception handling
        try {
            Comment invalidComment(0, "");  // Should throw FacebookException
            assert(false && "Test 5.1 failed: Should throw exception for invalid comment");
        } catch (const FacebookException& e) {
            assert(e.getType() == "ValidationError" && "Test 5.2 failed: Wrong exception type");
        }
        std::cout << "Test 5 passed: Exception handling" << std::endl;
        
        // Test 6: Reply management in Comment
        auto replyPtr = std::make_shared<Reply>(reply);
        comment.addReply(replyPtr);
        assert(comment.getReplies().size() == 1 && "Test 6.1 failed: Reply count mismatch");
        assert(comment.getReply(reply.getId())->getContent() == "You're welcome!" && 
               "Test 6.2 failed: Reply content mismatch");
        std::cout << "Test 6 passed: Reply management" << std::endl;
        
        // Cleanup
        fm.deleteFile(testFile);
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        // Cleanup in case of failure
        if (fm.fileExists(testFile)) {
            fm.deleteFile(testFile);
        }
        throw;
    }
}

void testMessageThreading() {
    std::cout << "\nTesting Message Threading..." << std::endl;
    
    try {
        // Test 7: Create a thread of messages
        Message msg1(1, 2, "Hi!");
        Message msg2(2, 1, "Hello!");
        Message msg3(1, 2, "How are you?");
        
        // Verify message properties
        assert(msg1.getSenderId() == 1 && msg1.getReceiverId() == 2 && "Test 7.1 failed: First message properties");
        assert(msg2.getSenderId() == 2 && msg2.getReceiverId() == 1 && "Test 7.2 failed: Second message properties");
        assert(msg3.getSenderId() == 1 && msg3.getReceiverId() == 2 && "Test 7.3 failed: Third message properties");
        std::cout << "Test 7 passed: Message threading" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        throw;
    }
}

void testCommentThread() {
    std::cout << "\nTesting Comment Thread..." << std::endl;
    
    try {
        // Test 8: Create a comment thread
        Comment mainComment(1, "Main comment");
        
        auto reply1 = std::make_shared<Reply>(2, mainComment.getId(), "First reply");
        auto reply2 = std::make_shared<Reply>(3, mainComment.getId(), "Second reply");
        auto reply3 = std::make_shared<Reply>(1, mainComment.getId(), "Third reply");
        
        mainComment.addReply(reply1);
        mainComment.addReply(reply2);
        mainComment.addReply(reply3);
        
        assert(mainComment.getReplies().size() == 3 && "Test 8.1 failed: Reply count mismatch");
        assert(reply1->getId() < reply2->getId() && "Test 8.2 failed: Reply IDs should be sequential");
        std::cout << "Test 8 passed: Comment thread" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        throw;
    }
}

int main() {
    try {
        testSocialInteractionFlow();
        testMessageThreading();
        testCommentThread();
        std::cout << "\nAll social interaction integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
