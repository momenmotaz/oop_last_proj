#include "../include/post.h"
#include <sstream>
#include <algorithm>

// Initialize static member
int Post::nextId = 1;

Post::Post(int authorId, const std::string& content, Privacy privacy)
    : id(nextId++), authorId(authorId), content(content), 
      timestamp(DateTime(2025, 1, 2, 1, 20, 32)),  // Current time from context
      privacy(privacy)
{
    if (!isValid()) {
        throw FacebookException("Invalid post parameters", "ValidationError");
    }
}

void Post::addComment(const std::shared_ptr<Comment>& comment) {
    if (!comment) {
        throw FacebookException("Comment cannot be null", "ValidationError");
    }
    comments.push_back(comment);
}

void Post::removeComment(int commentId) {
    auto it = std::find_if(comments.begin(), comments.end(),
                          [commentId](const auto& comment) { 
                              return comment->getId() == commentId; 
                          });
    
    if (it != comments.end()) {
        comments.erase(it);
    }
}

std::shared_ptr<Comment> Post::getComment(int commentId) const {
    auto it = std::find_if(comments.begin(), comments.end(),
                          [commentId](const auto& comment) { 
                              return comment->getId() == commentId; 
                          });
    
    return (it != comments.end()) ? *it : nullptr;
}

void Post::tagUser(int userId) {
    if (userId <= 0) {
        throw FacebookException("Invalid user ID for tagging", "ValidationError");
    }
    taggedUsers.insert(userId);
}

void Post::removeTag(int userId) {
    taggedUsers.erase(userId);
}

bool Post::isUserTagged(int userId) const {
    return taggedUsers.find(userId) != taggedUsers.end();
}

void Post::addLike(int userId) {
    if (userId <= 0) {
        throw FacebookException("Invalid user ID for like", "ValidationError");
    }
    likerIds.insert(userId);
}

void Post::removeLike(int userId) {
    likerIds.erase(userId);
}

bool Post::isLikedBy(int userId) const {
    return likerIds.find(userId) != likerIds.end();
}

bool Post::isVisibleTo(int userId) const {
    // For now, public posts are visible to everyone
    // Friends-only posts will need friend checking implementation later
    return privacy == Privacy::Public;
}

bool Post::isValid() const {
    return authorId > 0 && !content.empty();
}

std::string Post::toString() const {
    std::stringstream ss;
    ss << "Post #" << id << "\n"
       << "Author: User #" << authorId << "\n"
       << "Content: " << content << "\n"
       << "Time: " << timestamp.toString() << "\n"
       << "Privacy: " << (privacy == Privacy::Public ? "Public" : "Friends Only") << "\n"
       << "Likes: " << getLikesCount() << "\n"
       << "Comments: " << comments.size() << "\n"
       << "Tagged Users: " << taggedUsers.size();
    return ss.str();
}
