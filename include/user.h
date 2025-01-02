#ifndef USER_H
#define USER_H

#include "datetime.h"
#include "post.h"
#include "facebook_exception.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class User {
private:
    std::string email;
    std::string name;
    std::string password;
    std::string gender;
    DateTime birthdate;
    std::vector<Post*> posts;
    std::unordered_map<User*, bool> friends;  // bool indicates if restricted (true) or regular (false)

    bool isValidEmail(const std::string& email) const;
    void validateFields() const;
    std::string hashPassword(const std::string& password) const;

public:
    User(const std::string& email, const std::string& name, const std::string& password,
         const std::string& gender, const DateTime& birthdate);
    
    // Getters
    std::string getEmail() const { return email; }
    std::string getName() const { return name; }
    std::string getGender() const { return gender; }
    DateTime getBirthdate() const { return birthdate; }
    std::vector<Post*> getPosts() const { return posts; }
    
    // Password management
    bool validatePassword(const std::string& password) const;
    void changePassword(const std::string& oldPassword, const std::string& newPassword);
    
    // Friend management
    void addFriend(User* user, bool restricted = false);
    void removeFriend(User* user);
    bool isFriend(const User* user) const;
    bool isRestrictedFriend(const User* user) const;
    std::vector<User*> getFriends(bool restricted = false) const;
    
    // Post management
    void addPost(Post* post);
    void removePost(Post* post);
    std::vector<Post*> getVisiblePosts(const User* viewer) const;
    
    // Operator overloading
    std::vector<Post*> operator+(const User& other) const;  // Common posts
    std::vector<User*> operator&(const User& other) const;  // Mutual friends
    
    // User search
    static std::vector<User*> searchUsers(const std::vector<User*>& users, const std::string& query);
    
    // Serialization
    static User deserialize(const std::string& json);
    std::string serialize() const;
};

#endif // USER_H
