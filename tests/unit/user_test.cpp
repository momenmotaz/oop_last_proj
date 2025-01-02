#include "../../include/user.h"
#include "../../include/facebook_exception.h"
#include "../../include/datetime.h"
#include <cassert>
#include <iostream>

void testUserCreation() {
    std::cout << "Testing User Creation..." << std::endl;
    
    // Test 1: Create user with valid data
    User user1("john@example.com", "John Doe", "password123", "Male", DateTime(1990, 1, 1));
    assert(user1.getEmail() == "john@example.com" && "Test 1.1 failed: Email mismatch");
    assert(user1.getName() == "John Doe" && "Test 1.2 failed: Name mismatch");
    assert(user1.getGender() == "Male" && "Test 1.3 failed: Gender mismatch");
    
    // Test 2: Invalid email format
    try {
        User user2("invalid-email", "Jane Doe", "password123", "Female", DateTime(1995, 5, 15));
        assert(false && "Test 2 failed: Should throw exception for invalid email");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 2.1 failed: Wrong exception type");
    }
    
    // Test 3: Empty required fields
    try {
        User user3("", "", "", "", DateTime(2000, 1, 1));
        assert(false && "Test 3 failed: Should throw exception for empty fields");
    } catch (const FacebookException& e) {
        assert(e.getType() == "ValidationError" && "Test 3.1 failed: Wrong exception type");
    }
    
    std::cout << "User creation tests passed!" << std::endl;
}

void testPasswordManagement() {
    std::cout << "\nTesting Password Management..." << std::endl;
    
    User user("test@example.com", "Test User", "initial123", "Male", DateTime(1990, 1, 1));
    
    // Test 4: Password validation
    assert(user.validatePassword("initial123") && "Test 4.1 failed: Password validation failed");
    assert(!user.validatePassword("wrong123") && "Test 4.2 failed: Invalid password accepted");
    
    // Test 5: Password change
    user.changePassword("initial123", "newpass123");
    assert(user.validatePassword("newpass123") && "Test 5.1 failed: New password not set");
    assert(!user.validatePassword("initial123") && "Test 5.2 failed: Old password still valid");
    
    // Test 6: Invalid password change
    try {
        user.changePassword("wrongold", "newpass456");
        assert(false && "Test 6 failed: Should throw exception for wrong old password");
    } catch (const FacebookException& e) {
        assert(e.getType() == "AuthenticationError" && "Test 6.1 failed: Wrong exception type");
    }
    
    std::cout << "Password management tests passed!" << std::endl;
}

void testFriendshipManagement() {
    std::cout << "\nTesting Friendship Management..." << std::endl;
    
    User user1("user1@example.com", "User One", "pass123", "Male", DateTime(1990, 1, 1));
    User user2("user2@example.com", "User Two", "pass123", "Female", DateTime(1991, 2, 2));
    User user3("user3@example.com", "User Three", "pass123", "Male", DateTime(1992, 3, 3));
    
    // Test 7: Add regular friend
    user1.addFriend(&user2, false);  // regular friend
    assert(user1.isFriend(&user2) && "Test 7.1 failed: Friend not added");
    assert(!user1.isRestrictedFriend(&user2) && "Test 7.2 failed: Friend wrongly marked as restricted");
    
    // Test 8: Add restricted friend
    user1.addFriend(&user3, true);  // restricted friend
    assert(user1.isFriend(&user3) && "Test 8.1 failed: Restricted friend not added");
    assert(user1.isRestrictedFriend(&user3) && "Test 8.2 failed: Friend not marked as restricted");
    
    // Test 9: Remove friend
    user1.removeFriend(&user2);
    assert(!user1.isFriend(&user2) && "Test 9.1 failed: Friend not removed");
    
    std::cout << "Friendship management tests passed!" << std::endl;
}

void testSerialization() {
    std::cout << "\nTesting User Serialization..." << std::endl;
    
    // Test 10: Serialize and deserialize user
    User originalUser("test@example.com", "Test User", "pass123", "Male", DateTime(1990, 1, 1));
    std::string serialized = originalUser.serialize();
    User deserializedUser = User::deserialize(serialized);
    
    assert(deserializedUser.getEmail() == originalUser.getEmail() && "Test 10.1 failed: Email mismatch after deserialization");
    assert(deserializedUser.getName() == originalUser.getName() && "Test 10.2 failed: Name mismatch after deserialization");
    assert(deserializedUser.getGender() == originalUser.getGender() && "Test 10.3 failed: Gender mismatch after deserialization");
    
    std::cout << "Serialization tests passed!" << std::endl;
}

void testOperatorOverloading() {
    std::cout << "\nTesting Operator Overloading..." << std::endl;
    
    // Setup users
    User user1("user1@test.com", "User One", "pass123", "Male", DateTime(1990, 1, 1));
    User user2("user2@test.com", "User Two", "pass123", "Female", DateTime(1991, 2, 2));
    User user3("user3@test.com", "User Three", "pass123", "Male", DateTime(1992, 3, 3));
    
    // Add friends
    user1.addFriend(&user2);
    user1.addFriend(&user3);
    user2.addFriend(&user1);
    user2.addFriend(&user3);
    
    // Test 11: Common posts operator (+)
    Post* post1 = new Post(1, "Common post", Post::Privacy::Public, &user1);
    Post* post2 = new Post(2, "User1 only", Post::Privacy::Public, &user1);
    Post* post3 = new Post(3, "Another common", Post::Privacy::Public, &user2);
    
    user1.addPost(post1);
    user1.addPost(post2);
    user2.addPost(post1);  // Same post
    user2.addPost(post3);
    
    std::vector<Post*> commonPosts = user1 + user2;
    assert(commonPosts.size() == 1 && "Test 11.1 failed: Should have one common post");
    assert(commonPosts[0]->getId() == 1 && "Test 11.2 failed: Wrong common post");
    
    // Test 12: Mutual friends operator (&)
    std::vector<User*> mutualFriends = user1 & user2;
    assert(mutualFriends.size() == 1 && "Test 12.1 failed: Should have one mutual friend");
    assert(mutualFriends[0]->getEmail() == "user3@test.com" && "Test 12.2 failed: Wrong mutual friend");
    
    // Cleanup
    delete post1;
    delete post2;
    delete post3;
    
    std::cout << "Operator overloading tests passed!" << std::endl;
}

void testUserSearch() {
    std::cout << "\nTesting User Search..." << std::endl;
    
    // Setup users
    User user1("john@test.com", "John Smith", "pass123", "Male", DateTime(1990, 1, 1));
    User user2("jane@test.com", "Jane Smith", "pass123", "Female", DateTime(1991, 2, 2));
    User user3("bob@test.com", "Bob Wilson", "pass123", "Male", DateTime(1992, 3, 3));
    
    std::vector<User*> allUsers = {&user1, &user2, &user3};
    
    // Test 13: Search by name
    std::vector<User*> smiths = User::searchUsers(allUsers, "Smith");
    assert(smiths.size() == 2 && "Test 13.1 failed: Should find two Smiths");
    
    // Test 14: Search by email
    std::vector<User*> byEmail = User::searchUsers(allUsers, "bob@test.com");
    assert(byEmail.size() == 1 && "Test 14.1 failed: Should find one user by email");
    assert(byEmail[0]->getName() == "Bob Wilson" && "Test 14.2 failed: Wrong user found");
    
    // Test 15: Case insensitive search
    std::vector<User*> caseInsensitive = User::searchUsers(allUsers, "SMITH");
    assert(caseInsensitive.size() == 2 && "Test 15.1 failed: Case insensitive search failed");
    
    std::cout << "User search tests passed!" << std::endl;
}

int main() {
    try {
        testUserCreation();
        testPasswordManagement();
        testFriendshipManagement();
        testSerialization();
        testOperatorOverloading();
        testUserSearch();
        
        std::cout << "\nAll User tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
