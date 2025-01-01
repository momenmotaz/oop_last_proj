#include "../include/comment.h"
#include "../include/reply.h"
#include <sstream>
#include <algorithm>

// Initialize static member
int Comment::nextId = 1;

Comment::Comment(int authorId, const std::string& content)
    : id(nextId++), authorId(authorId), content(content),
      timestamp(DateTime(2025, 1, 2, 0, 47, 41))  // Current time from context
{
    if (!isValid()) {
        throw FacebookException("Invalid comment parameters", "ValidationError");
    }
}

void Comment::addReply(const std::shared_ptr<Reply>& reply) {
    if (!reply) {
        throw FacebookException("Reply cannot be null", "ValidationError");
    }
    
    if (reply->getCommentId() != id) {
        throw FacebookException("Reply belongs to a different comment", "ValidationError");
    }
    
    replies.push_back(reply);
}

void Comment::removeReply(int replyId) {
    auto it = std::find_if(replies.begin(), replies.end(),
                          [replyId](const auto& reply) { return reply->getId() == replyId; });
    
    if (it != replies.end()) {
        replies.erase(it);
    }
}

std::shared_ptr<Reply> Comment::getReply(int replyId) const {
    auto it = std::find_if(replies.begin(), replies.end(),
                          [replyId](const auto& reply) { return reply->getId() == replyId; });
    
    if (it != replies.end()) {
        return *it;
    }
    return nullptr;
}

void Comment::addLike(int userId) {
    if (userId <= 0) {
        throw FacebookException("Invalid user ID", "ValidationError");
    }
    
    if (!isLikedBy(userId)) {
        likerIds.push_back(userId);
    }
}

void Comment::removeLike(int userId) {
    auto it = std::find(likerIds.begin(), likerIds.end(), userId);
    if (it != likerIds.end()) {
        likerIds.erase(it);
    }
}

bool Comment::isLikedBy(int userId) const {
    return std::find(likerIds.begin(), likerIds.end(), userId) != likerIds.end();
}

bool Comment::isValid() const {
    return authorId > 0 && !content.empty();
}

std::string Comment::toString() const {
    std::stringstream ss;
    ss << "Comment #" << id << " by User #" << authorId << "\n"
       << "Content: " << content << "\n"
       << "Time: " << timestamp.toString() << "\n"
       << "Likes: " << getLikesCount() << "\n"
       << "Replies: " << replies.size();
    return ss.str();
}

bool Comment::operator<(const Comment& other) const {
    return id < other.id;
}

bool Comment::operator>(const Comment& other) const {
    return id > other.id;
}

bool Comment::operator==(const Comment& other) const {
    return id == other.id;
}
