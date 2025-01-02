#include "../include/user.h"
#include <regex>
#include <algorithm>
#include <sstream>

bool User::isValidEmail(const std::string& email) const {
    // Basic email validation using regex
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

void User::validateFields() const {
    std::vector<std::string> errors;
    
    if (email.empty()) errors.push_back("Email is required");
    else if (!isValidEmail(email)) errors.push_back("Invalid email format");
    
    if (name.empty()) errors.push_back("Name is required");
    if (password.empty()) errors.push_back("Password is required");
    if (gender.empty()) errors.push_back("Gender is required");
    
    if (!errors.empty()) {
        std::string errorMsg;
        for (const auto& error : errors) {
            if (!errorMsg.empty()) errorMsg += ", ";
            errorMsg += error;
        }
        throw FacebookException(errorMsg, "ValidationError");
    }
}

std::string User::hashPassword(const std::string& password) const {
    // Simple hash function for demonstration (in real-world, use proper hashing)
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

User::User(const std::string& email, const std::string& name, const std::string& password,
           const std::string& gender, const DateTime& birthdate)
    : email(email), name(name), gender(gender), birthdate(birthdate) {
    this->password = hashPassword(password);
    validateFields();
}

bool User::validatePassword(const std::string& password) const {
    return this->password == hashPassword(password);
}

void User::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!validatePassword(oldPassword)) {
        throw FacebookException("Invalid old password", "AuthenticationError");
    }
    password = hashPassword(newPassword);
}

void User::addFriend(User* user, bool restricted) {
    if (user && user != this) {
        friends[user] = restricted;
    }
}

void User::removeFriend(User* user) {
    if (user) {
        friends.erase(user);
    }
}

bool User::isFriend(const User* user) const {
    return friends.find(const_cast<User*>(user)) != friends.end();
}

bool User::isRestrictedFriend(const User* user) const {
    auto it = friends.find(const_cast<User*>(user));
    return it != friends.end() && it->second;
}

std::vector<User*> User::getFriends(bool restricted) const {
    std::vector<User*> result;
    for (const auto& pair : friends) {
        if (pair.second == restricted) {
            result.push_back(pair.first);
        }
    }
    return result;
}

void User::addPost(Post* post) {
    if (post) {
        posts.push_back(post);
    }
}

void User::removePost(Post* post) {
    auto it = std::find(posts.begin(), posts.end(), post);
    if (it != posts.end()) {
        posts.erase(it);
    }
}

std::vector<Post*> User::getVisiblePosts(const User* viewer) const {
    std::vector<Post*> visiblePosts;
    
    for (Post* post : posts) {
        if (post->getPrivacy() == Post::Privacy::Public || 
            viewer == this || 
            (isFriend(viewer) && !isRestrictedFriend(viewer))) {
            visiblePosts.push_back(post);
        }
    }
    
    return visiblePosts;
}

User User::deserialize(const std::string& data) {
    std::istringstream ss(data);
    std::string email, name, password, gender, birthdateStr;
    
    // Format: email|name|password|gender|birthdate
    std::getline(ss, email, '|');
    std::getline(ss, name, '|');
    std::getline(ss, password, '|');
    std::getline(ss, gender, '|');
    std::getline(ss, birthdateStr);
    
    DateTime birthdate = DateTime::deserialize(birthdateStr);
    
    return User(email, name, password, gender, birthdate);
}

std::string User::serialize() const {
    std::ostringstream ss;
    ss << email << '|'
       << name << '|'
       << password << '|'
       << gender << '|'
       << birthdate.serialize();
    return ss.str();
}

std::vector<Post*> User::operator+(const User& other) const {
    std::vector<Post*> commonPosts;
    
    for (Post* myPost : posts) {
        for (Post* theirPost : other.posts) {
            if (myPost == theirPost) {  // Same post object
                commonPosts.push_back(myPost);
            }
        }
    }
    
    return commonPosts;
}

std::vector<User*> User::operator&(const User& other) const {
    std::vector<User*> mutualFriends;
    
    for (const auto& myFriend : friends) {
        User* user = myFriend.first;
        if (other.isFriend(user) && user != this && user != &other) {
            mutualFriends.push_back(user);
        }
    }
    
    return mutualFriends;
}

std::vector<User*> User::searchUsers(const std::vector<User*>& users, const std::string& query) {
    std::vector<User*> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (User* user : users) {
        // Convert user data to lower case for case-insensitive comparison
        std::string lowerName = user->getName();
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        std::string lowerEmail = user->getEmail();
        std::transform(lowerEmail.begin(), lowerEmail.end(), lowerEmail.begin(), ::tolower);
        
        // Check if query matches name or email
        if (lowerName.find(lowerQuery) != std::string::npos ||
            lowerEmail.find(lowerQuery) != std::string::npos) {
            results.push_back(user);
        }
    }
    
    return results;
}
