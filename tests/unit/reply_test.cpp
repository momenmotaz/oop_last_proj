#include "../../include/reply.h"
#include <cassert>
#include <iostream>

void testReplyCreation() {
    std::cout << "Testing Reply Creation..." << std::endl;
    
    // Test 1: Valid reply creation
    Reply reply(1, 1, "This is a reply");
    assert(reply.getAuthorId() == 1 && "Test 1.1 failed: Author ID mismatch");
    assert(reply.getCommentId() == 1 && "Test 1.2 failed: Comment ID mismatch");
    assert(reply.getContent() == "This is a reply" && "Test 1.3 failed: Content mismatch");
    std::cout << "Test 1 passed: Valid reply creation" << std::endl;
    
    // Test 2: Invalid reply creation
    try {
        Reply invalid(0, 1, "");  // Invalid author ID and empty content
        assert(false && "Test 2.1 failed: Should throw exception for invalid parameters");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.2 failed: Wrong exception type");
    }
    std::cout << "Test 2 passed: Invalid reply creation" << std::endl;
}

void testLikeManagement() {
    std::cout << "\nTesting Like Management..." << std::endl;
    
    Reply reply(1, 1, "Test reply");
    
    // Test 3: Add likes
    reply.addLike(2);
    assert(reply.getLikesCount() == 1 && "Test 3.1 failed: Like count mismatch");
    assert(reply.isLikedBy(2) && "Test 3.2 failed: Like not found");
    
    // Add same like again
    reply.addLike(2);
    assert(reply.getLikesCount() == 1 && "Test 3.3 failed: Duplicate like should not increase count");
    std::cout << "Test 3 passed: Add likes" << std::endl;
    
    // Test 4: Remove likes
    reply.removeLike(2);
    assert(reply.getLikesCount() == 0 && "Test 4.1 failed: Like count after removal");
    assert(!reply.isLikedBy(2) && "Test 4.2 failed: Like should be removed");
    std::cout << "Test 4 passed: Remove likes" << std::endl;
    
    // Test 5: Invalid like operations
    try {
        reply.addLike(0);  // Invalid user ID
        assert(false && "Test 5.1 failed: Should throw exception for invalid user ID");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 5.2 failed: Wrong exception type");
    }
    std::cout << "Test 5 passed: Invalid like operations" << std::endl;
}

void testComparison() {
    std::cout << "\nTesting Comparison Operations..." << std::endl;
    
    // Test 6: Comparison by ID
    Reply reply1(1, 1, "First reply");
    Reply reply2(2, 1, "Second reply");
    
    assert(reply1.getId() < reply2.getId() && "Test 6.1 failed: First reply should have lower ID");
    assert(!(reply1 == reply2) && "Test 6.2 failed: Different replies should not be equal");
    std::cout << "Test 6 passed: Comparison operators" << std::endl;
}

int main() {
    try {
        testReplyCreation();
        testLikeManagement();
        testComparison();
        std::cout << "\nAll Reply tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
