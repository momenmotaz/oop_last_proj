#include "../../include/post.h"
#include "../../include/comment.h"
#include <cassert>
#include <iostream>
#include <memory>

// Test helper function
void assertPostEquals(const Post& post, int authorId, const std::string& content, 
                     Post::Privacy privacy = Post::Privacy::Public) {
    assert(post.getAuthorId() == authorId && "Author ID mismatch");
    assert(post.getContent() == content && "Content mismatch");
    assert(post.getPrivacy() == privacy && "Privacy setting mismatch");
}

void testPostCreation() {
    std::cout << "Testing Post Creation..." << std::endl;
    
    // Test 1: Create public post
    Post post1(1, "Hello, World!");
    assertPostEquals(post1, 1, "Hello, World!");
    std::cout << "Test 1 passed: Public post creation" << std::endl;
    
    // Test 2: Create friends-only post
    Post post2(1, "Friends only post", Post::Privacy::FriendsOnly);
    assertPostEquals(post2, 1, "Friends only post", Post::Privacy::FriendsOnly);
    std::cout << "Test 2 passed: Friends-only post creation" << std::endl;
    
    // Test 3: Invalid post creation
    try {
        Post invalid(0, "");  // Invalid author ID and empty content
        assert(false && "Test 3.1 failed: Should throw exception for invalid parameters");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 3.2 failed: Wrong exception type");
    }
    std::cout << "Test 3 passed: Invalid post creation" << std::endl;
}

void testCommentManagement() {
    std::cout << "\nTesting Comment Management..." << std::endl;
    
    Post post(1, "Test post");
    
    // Test 4: Add comments
    auto comment1 = std::make_shared<Comment>(2, "First comment");
    auto comment2 = std::make_shared<Comment>(3, "Second comment");
    
    post.addComment(comment1);
    post.addComment(comment2);
    
    assert(post.getComments().size() == 2 && "Test 4.1 failed: Comment count mismatch");
    assert(post.getComment(comment1->getId())->getContent() == "First comment" && 
           "Test 4.2 failed: First comment content mismatch");
    std::cout << "Test 4 passed: Add comments" << std::endl;
    
    // Test 5: Remove comment
    post.removeComment(comment1->getId());
    assert(post.getComments().size() == 1 && "Test 5.1 failed: Comment count after removal");
    assert(!post.getComment(comment1->getId()) && "Test 5.2 failed: Comment should be removed");
    std::cout << "Test 5 passed: Remove comment" << std::endl;
}

void testTaggedUsers() {
    std::cout << "\nTesting Tagged Users..." << std::endl;
    
    Post post(1, "Test post with tags");
    
    // Test 6: Tag users
    post.tagUser(2);
    post.tagUser(3);
    
    assert(post.getTaggedUsers().size() == 2 && "Test 6.1 failed: Tagged users count mismatch");
    assert(post.isUserTagged(2) && "Test 6.2 failed: User 2 should be tagged");
    assert(post.isUserTagged(3) && "Test 6.3 failed: User 3 should be tagged");
    std::cout << "Test 6 passed: Tag users" << std::endl;
    
    // Test 7: Remove tag
    post.removeTag(2);
    assert(post.getTaggedUsers().size() == 1 && "Test 7.1 failed: Tagged users count after removal");
    assert(!post.isUserTagged(2) && "Test 7.2 failed: User 2 should not be tagged");
    std::cout << "Test 7 passed: Remove tag" << std::endl;
    
    // Test 8: Invalid tags
    try {
        post.tagUser(0);  // Invalid user ID
        assert(false && "Test 8.1 failed: Should throw exception for invalid user ID");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 8.2 failed: Wrong exception type");
    }
    std::cout << "Test 8 passed: Invalid tags" << std::endl;
}

void testLikes() {
    std::cout << "\nTesting Likes..." << std::endl;
    
    Post post(1, "Test post for likes");
    
    // Test 9: Add likes
    post.addLike(2);
    post.addLike(3);
    
    assert(post.getLikesCount() == 2 && "Test 9.1 failed: Likes count mismatch");
    assert(post.isLikedBy(2) && "Test 9.2 failed: User 2 should like the post");
    
    // Try to add same like again
    post.addLike(2);
    assert(post.getLikesCount() == 2 && "Test 9.3 failed: Duplicate like should not increase count");
    std::cout << "Test 9 passed: Add likes" << std::endl;
    
    // Test 10: Remove like
    post.removeLike(2);
    assert(post.getLikesCount() == 1 && "Test 10.1 failed: Likes count after removal");
    assert(!post.isLikedBy(2) && "Test 10.2 failed: Like should be removed");
    std::cout << "Test 10 passed: Remove like" << std::endl;
}

void testPrivacy() {
    std::cout << "\nTesting Privacy Settings..." << std::endl;
    
    // Test 11: Privacy checks
    Post publicPost(1, "Public post", Post::Privacy::Public);
    Post friendsPost(1, "Friends only post", Post::Privacy::FriendsOnly);
    
    assert(publicPost.isVisibleTo(2) && "Test 11.1 failed: Public post should be visible to all");
    assert(publicPost.isVisibleTo(3) && "Test 11.2 failed: Public post should be visible to all");
    
    // For friends-only post, we'll need to implement friend checking later
    assert(friendsPost.getPrivacy() == Post::Privacy::FriendsOnly && 
           "Test 11.3 failed: Privacy setting mismatch");
    std::cout << "Test 11 passed: Privacy settings" << std::endl;
}

int main() {
    try {
        testPostCreation();
        testCommentManagement();
        testTaggedUsers();
        testLikes();
        testPrivacy();
        std::cout << "\nAll Post tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
