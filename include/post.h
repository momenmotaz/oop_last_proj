#ifndef POST_H
#define POST_H

#include <string>
#include <vector>
#include <unordered_map>
#include "datetime.h"

class User;
class Comment;

class Post {
public:
    enum class Privacy {
        Public,
        FriendsOnly
    };

private:
    int id;
    std::string content;
    Privacy privacy;
    User* author;
    DateTime createdAt;
    std::vector<User*> taggedUsers;
    std::vector<Comment*> comments;
    std::unordered_map<User*, int> reactions;  // User -> reaction type

public:
    Post(int id, const std::string& content, Privacy privacy, User* author);
    
    // Getters
    int getId() const { return id; }
    std::string getContent() const { return content; }
    Privacy getPrivacy() const { return privacy; }
    User* getAuthor() const { return author; }
    DateTime getCreatedAt() const { return createdAt; }
    
    // Tag management
    void tagUser(User* user);
    void untagUser(User* user);
    bool isUserTagged(const User* user) const;
    std::vector<User*> getTaggedUsers() const { return taggedUsers; }
    
    // Comment management
    void addComment(Comment* comment);
    void removeComment(Comment* comment);
    std::vector<Comment*> getComments() const { return comments; }
    
    // Reaction management
    void addReaction(User* user, int reactionType);
    void removeReaction(User* user);
    int getReactionType(const User* user) const;
    std::unordered_map<User*, int> getReactions() const { return reactions; }
    
    // Serialization
    static Post deserialize(const std::string& json);
    std::string serialize() const;
};

#endif // POST_H
