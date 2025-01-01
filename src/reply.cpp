#include "../include/reply.h"
#include <sstream>
#include <algorithm>

// Initialize static member
int Reply::nextId = 1;

Reply::Reply(int authorId, int commentId, const std::string& content)
    : id(nextId++), authorId(authorId), commentId(commentId), 
      content(content), timestamp(DateTime(2025, 1, 2, 0, 47, 41))  // Current time from context
{
    if (!isValid()) {
        throw FacebookException("Invalid reply parameters", "ValidationError");
    }
}

void Reply::addLike(int userId) {
    if (userId <= 0) {
        throw FacebookException("Invalid user ID", "ValidationError");
    }
    
    if (!isLikedBy(userId)) {
        likerIds.push_back(userId);
    }
}

void Reply::removeLike(int userId) {
    auto it = std::find(likerIds.begin(), likerIds.end(), userId);
    if (it != likerIds.end()) {
        likerIds.erase(it);
    }
}

bool Reply::isLikedBy(int userId) const {
    return std::find(likerIds.begin(), likerIds.end(), userId) != likerIds.end();
}

bool Reply::isValid() const {
    return authorId > 0 && 
           commentId > 0 && 
           !content.empty();
}

std::string Reply::toString() const {
    std::stringstream ss;
    ss << "Reply #" << id << " by User #" << authorId << "\n"
       << "On Comment #" << commentId << "\n"
       << "Content: " << content << "\n"
       << "Time: " << timestamp.toString() << "\n"
       << "Likes: " << getLikesCount();
    return ss.str();
}

bool Reply::operator<(const Reply& other) const {
    return id < other.id;
}

bool Reply::operator>(const Reply& other) const {
    return id > other.id;
}

bool Reply::operator==(const Reply& other) const {
    return id == other.id;
}
