#include "../../include/comment.h"
#include "../../include/reply.h"
#include <cassert>
#include <iostream>
#include <memory>

void testCommentCreation() {
    std::cout << "Testing Comment Creation..." << std::endl;
    
    // Test 1: Valid comment creation
    Comment comment(1, "This is a comment");
    assert(comment.getAuthorId() == 1 && "Test 1.1 failed: Author ID mismatch");
    assert(comment.getContent() == "This is a comment" && "Test 1.2 failed: Content mismatch");
    std::cout << "Test 1 passed: Valid comment creation" << std::endl;
    
    // Test 2: Invalid comment creation
    try {
        Comment invalid(0, "");  // Invalid author ID and empty content
        assert(false && "Test 2.1 failed: Should throw exception for invalid parameters");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.2 failed: Wrong exception type");
    }
    std::cout << "Test 2 passed: Invalid comment creation" << std::endl;
}

void testReplyManagement() {
    std::cout << "\nTesting Reply Management..." << std::endl;
    
    Comment comment(1, "Test comment");
    
    // Test 3: Add reply
    auto reply = std::make_shared<Reply>(2, comment.getId(), "Test reply");
    comment.addReply(reply);
    assert(comment.getReplies().size() == 1 && "Test 3.1 failed: Reply count mismatch");
    
    auto retrievedReply = comment.getReply(reply->getId());
    assert(retrievedReply && retrievedReply->getContent() == "Test reply" && 
           "Test 3.2 failed: Retrieved reply mismatch");
    std::cout << "Test 3 passed: Add reply" << std::endl;
    
    // Test 4: Remove reply
    comment.removeReply(reply->getId());
    assert(comment.getReplies().empty() && "Test 4.1 failed: Reply should be removed");
    assert(!comment.getReply(reply->getId()) && "Test 4.2 failed: Should not find removed reply");
    std::cout << "Test 4 passed: Remove reply" << std::endl;
    
    // Test 5: Invalid reply operations
    try {
        comment.addReply(nullptr);
        assert(false && "Test 5.1 failed: Should throw exception for null reply");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 5.2 failed: Wrong exception type");
    }
    
    try {
        auto invalidReply = std::make_shared<Reply>(2, comment.getId() + 1, "Invalid reply");
        comment.addReply(invalidReply);
        assert(false && "Test 5.3 failed: Should throw exception for reply with wrong comment ID");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 5.4 failed: Wrong exception type");
    }
    std::cout << "Test 5 passed: Invalid reply operations" << std::endl;
}

void testLikeManagement() {
    std::cout << "\nTesting Like Management..." << std::endl;
    
    Comment comment(1, "Test comment");
    
    // Test 6: Add likes
    comment.addLike(2);
    assert(comment.getLikesCount() == 1 && "Test 6.1 failed: Like count mismatch");
    assert(comment.isLikedBy(2) && "Test 6.2 failed: Like not found");
    
    // Add same like again
    comment.addLike(2);
    assert(comment.getLikesCount() == 1 && "Test 6.3 failed: Duplicate like should not increase count");
    std::cout << "Test 6 passed: Add likes" << std::endl;
    
    // Test 7: Remove likes
    comment.removeLike(2);
    assert(comment.getLikesCount() == 0 && "Test 7.1 failed: Like count after removal");
    assert(!comment.isLikedBy(2) && "Test 7.2 failed: Like should be removed");
    std::cout << "Test 7 passed: Remove likes" << std::endl;
}

void testComparison() {
    std::cout << "\nTesting Comparison Operations..." << std::endl;
    
    // Test 8: Comparison by ID
    Comment comment1(1, "First comment");
    Comment comment2(2, "Second comment");
    
    assert(comment1.getId() < comment2.getId() && "Test 8.1 failed: First comment should have lower ID");
    assert(!(comment1 == comment2) && "Test 8.2 failed: Different comments should not be equal");
    std::cout << "Test 8 passed: Comparison operators" << std::endl;
}

int main() {
    try {
        testCommentCreation();
        testReplyManagement();
        testLikeManagement();
        testComparison();
        std::cout << "\nAll Comment tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
