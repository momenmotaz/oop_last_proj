#ifndef REPLY_H
#define REPLY_H

#include "datetime.h"
#include "facebook_exception.h"
#include <string>
#include <vector>

class Reply {
private:
    int id;
    int authorId;
    int commentId;  // ID of the parent comment
    std::string content;
    DateTime timestamp;
    std::vector<int> likerIds;  // IDs of users who liked this reply
    static int nextId;  // For generating unique IDs

public:
    // Constructor
    Reply(int authorId, int commentId, const std::string& content);
    
    // Getters
    int getId() const { return id; }
    int getAuthorId() const { return authorId; }
    int getCommentId() const { return commentId; }
    const std::string& getContent() const { return content; }
    const DateTime& getTimestamp() const { return timestamp; }
    const std::vector<int>& getLikerIds() const { return likerIds; }
    
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
    bool operator<(const Reply& other) const;  // Compare by timestamp
    bool operator>(const Reply& other) const;
    bool operator==(const Reply& other) const;
};

#endif // REPLY_H
