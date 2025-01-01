#include "../../include/post.h"
#include "../../include/message.h"
#include "../../include/comment.h"
#include "../../include/reply.h"
#include "../../include/file_manager.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

// Helper function to create a complete social interaction scenario
void createSocialInteraction(Post& post, Message& msg) {
    // Add comments to post
    auto comment1 = std::make_shared<Comment>(2, "Great post!");
    auto comment2 = std::make_shared<Comment>(3, "I agree!");
    
    // Add replies to first comment
    auto reply1 = std::make_shared<Reply>(4, comment1->getId(), "Thanks!");
    auto reply2 = std::make_shared<Reply>(5, comment1->getId(), "Me too!");
    
    // Build the interaction structure
    comment1->addReply(reply1);
    comment1->addReply(reply2);
    post.addComment(comment1);
    post.addComment(comment2);
    
    // Add likes
    post.addLike(2);
    post.addLike(3);
    comment1->addLike(3);
    comment1->addLike(4);
    reply1->addLike(5);
    
    // Tag users
    post.tagUser(2);
    post.tagUser(3);
}

// Helper function to verify the social interaction structure
void verifySocialInteraction(const Post& post, const Message& msg) {
    // Verify post structure
    assert(post.getComments().size() == 2 && "Post should have 2 comments");
    assert(post.getLikesCount() == 2 && "Post should have 2 likes");
    assert(post.getTaggedUsers().size() == 2 && "Post should have 2 tagged users");
    
    // Verify first comment and its replies
    auto comment1 = post.getComments()[0];
    assert(comment1->getReplies().size() == 2 && "First comment should have 2 replies");
    assert(comment1->getLikesCount() == 2 && "First comment should have 2 likes");
    
    // Verify first reply
    auto reply1 = comment1->getReplies()[0];
    assert(reply1->getLikesCount() == 1 && "First reply should have 1 like");
}

void testFileSystemIntegration() {
    std::cout << "Testing File System Integration..." << std::endl;
    
    FileManager& fm = FileManager::getInstance();
    std::string testFile = "test_social_interaction.txt";
    
    try {
        // Create a complete social interaction
        Post post(1, "Hello world!", Post::Privacy::Public);
        Message msg(1, 2, "Check out my new post!");
        createSocialInteraction(post, msg);
        
        // Serialize and save to file
        std::stringstream ss;
        ss << "POST\n" << post.toString() << "\n"
           << "MESSAGE\n" << msg.toString();
        
        fm.writeFile(testFile, ss.str());
        std::cout << "Test 1 passed: Saved social interaction to file" << std::endl;
        
        // Read and verify
        std::string content = fm.readFile(testFile);
        assert(!content.empty() && "File content should not be empty");
        assert(content.find("POST") != std::string::npos && "File should contain post data");
        assert(content.find("MESSAGE") != std::string::npos && "File should contain message data");
        std::cout << "Test 2 passed: Read social interaction from file" << std::endl;
        
        // Cleanup
        fm.deleteFile(testFile);
        std::cout << "Test 3 passed: Cleaned up test file" << std::endl;
        
    } catch (const FacebookException& e) {
        std::cerr << "FacebookException: " << e.what() << " (Type: " << e.getType() << ")" << std::endl;
        throw;
    }
}

void testTimeBasedFeatures() {
    std::cout << "\nTesting Time-Based Features..." << std::endl;
    
    try {
        // Create a post and message at different times
        Post post(1, "First post!");
        DateTime postTime = post.getTimestamp();
        
        Message msg(1, 2, "First message!");
        DateTime msgTime = msg.getTimestamp();
        
        // Verify timestamps
        assert(!postTime.toString().empty() && "Post timestamp should not be empty");
        assert(!msgTime.toString().empty() && "Message timestamp should not be empty");
        std::cout << "Test 4 passed: Timestamp creation" << std::endl;
        
    } catch (const FacebookException& e) {
        std::cerr << "FacebookException: " << e.what() << " (Type: " << e.getType() << ")" << std::endl;
        throw;
    }
}

void testExceptionHandling() {
    std::cout << "\nTesting Exception Handling..." << std::endl;
    
    try {
        // Test invalid post
        try {
            Post invalidPost(0, "");
            assert(false && "Should throw exception for invalid post");
        } catch (const FacebookException& e) {
            assert(e.getType() == "ValidationError" && "Should be validation error");
        }
        std::cout << "Test 5 passed: Invalid post handling" << std::endl;
        
        // Test invalid message
        try {
            Message invalidMsg(0, 0, "");
            assert(false && "Should throw exception for invalid message");
        } catch (const FacebookException& e) {
            assert(e.getType() == "ValidationError" && "Should be validation error");
        }
        std::cout << "Test 6 passed: Invalid message handling" << std::endl;
        
        // Test invalid comment
        try {
            Comment invalidComment(0, "");
            assert(false && "Should throw exception for invalid comment");
        } catch (const FacebookException& e) {
            assert(e.getType() == "ValidationError" && "Should be validation error");
        }
        std::cout << "Test 7 passed: Invalid comment handling" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        throw;
    }
}

void testComplexInteraction() {
    std::cout << "\nTesting Complex Social Interaction..." << std::endl;
    
    try {
        // Create main post
        Post post(1, "Check out this awesome post!");
        Message notification(1, 2, "New post alert!");
        
        // Create social interaction
        createSocialInteraction(post, notification);
        
        // Verify structure
        verifySocialInteraction(post, notification);
        std::cout << "Test 8 passed: Complex social interaction" << std::endl;
        
        // Test post privacy
        assert(post.isVisibleTo(2) && "Public post should be visible to user 2");
        Post privatePost(1, "Friends only!", Post::Privacy::FriendsOnly);
        std::cout << "Test 9 passed: Privacy settings" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        throw;
    }
}

int main() {
    try {
        testFileSystemIntegration();
        testTimeBasedFeatures();
        testExceptionHandling();
        testComplexInteraction();
        
        std::cout << "\nAll social system integration tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
