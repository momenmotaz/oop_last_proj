#ifndef COMMENT_H
#define COMMENT_H

#include "datetime.h"
#include "facebook_exception.h"
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class User;
class Reply;

class Comment {
private:
    int id;
    int authorId;
    std::string content;
    DateTime timestamp;
    std::vector<std::shared_ptr<Reply>> replies;
    std::vector<int> likerIds;  // IDs of users who liked this comment
    static int nextId;  // For generating unique IDs

public:
    // Constructor
    Comment(int authorId, const std::string& content);
    
    // Getters
    int getId() const { return id; }
    int getAuthorId() const { return authorId; }
    const std::string& getContent() const { return content; }
    const DateTime& getTimestamp() const { return timestamp; }
    const std::vector<std::shared_ptr<Reply>>& getReplies() const { return replies; }
    const std::vector<int>& getLikerIds() const { return likerIds; }
    
    // Reply management
    void addReply(const std::shared_ptr<Reply>& reply);
    void removeReply(int replyId);
    std::shared_ptr<Reply> getReply(int replyId) const;
    
    // Like management
    void addLike(int userId);
    void removeLike(int userId);
    bool isLikedBy(int userId) const;
    int getLikesCount() const { return likerIds.size(); }
    
    // Validation
    bool isValid() const;
    
    // String representation
    std::string toString() const;
    
    // Comparison operators
    bool operator<(const Comment& other) const;  // Compare by timestamp
    bool operator>(const Comment& other) const;
    bool operator==(const Comment& other) const;
};

#endif // COMMENT_H
