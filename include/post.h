#ifndef POST_H
#define POST_H

#include "datetime.h"
#include "comment.h"
#include "facebook_exception.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

class Post {
public:
    // Privacy enum
    enum class Privacy {
        Public,
        FriendsOnly
    };

private:
    int id;
    int authorId;
    std::string content;
    DateTime timestamp;
    Privacy privacy;
    std::vector<std::shared_ptr<Comment>> comments;
    std::unordered_set<int> taggedUsers;
    std::unordered_set<int> likerIds;
    static int nextId;

public:
    // Constructors
    Post(int authorId, const std::string& content, Privacy privacy = Privacy::Public);
    
    // Getters
    int getId() const { return id; }
    int getAuthorId() const { return authorId; }
    const std::string& getContent() const { return content; }
    const DateTime& getTimestamp() const { return timestamp; }
    Privacy getPrivacy() const { return privacy; }
    
    // Comment management
    void addComment(const std::shared_ptr<Comment>& comment);
    void removeComment(int commentId);
    std::shared_ptr<Comment> getComment(int commentId) const;
    const std::vector<std::shared_ptr<Comment>>& getComments() const { return comments; }
    
    // Tagged users management
    void tagUser(int userId);
    void removeTag(int userId);
    bool isUserTagged(int userId) const;
    const std::unordered_set<int>& getTaggedUsers() const { return taggedUsers; }
    
    // Like management
    void addLike(int userId);
    void removeLike(int userId);
    bool isLikedBy(int userId) const;
    int getLikesCount() const { return likerIds.size(); }
    const std::unordered_set<int>& getLikerIds() const { return likerIds; }
    
    // Privacy management
    bool isVisibleTo(int userId) const;
    void setPrivacy(Privacy newPrivacy) { privacy = newPrivacy; }
    
    // Validation
    bool isValid() const;
    
    // String representation
    std::string toString() const;
    
    // Comparison operators
    bool operator<(const Post& other) const { return id < other.id; }
    bool operator>(const Post& other) const { return id > other.id; }
    bool operator==(const Post& other) const { return id == other.id; }
};

#endif // POST_H
