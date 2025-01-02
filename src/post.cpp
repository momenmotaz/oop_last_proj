#include "../include/post.h"
#include "../include/user.h"
#include "../include/comment.h"
#include "../include/facebook_exception.h"
#include <sstream>
#include <algorithm>

Post::Post(int id, const std::string& content, Privacy privacy, User* author)
    : id(id), content(content), privacy(privacy), author(author), 
      createdAt(DateTime(2025, 1, 2, 2, 37, 2)) {  // Current time from context
    if (!author || content.empty()) {
        throw FacebookException("Invalid post parameters", "ValidationError");
    }
}

void Post::tagUser(User* user) {
    if (!user) {
        throw FacebookException("Cannot tag null user", "ValidationError");
    }
    taggedUsers.push_back(user);
}

void Post::untagUser(User* user) {
    auto it = std::find(taggedUsers.begin(), taggedUsers.end(), user);
    if (it != taggedUsers.end()) {
        taggedUsers.erase(it);
    }
}

bool Post::isUserTagged(const User* user) const {
    return std::find(taggedUsers.begin(), taggedUsers.end(), user) != taggedUsers.end();
}

void Post::addComment(Comment* comment) {
    if (!comment) {
        throw FacebookException("Cannot add null comment", "ValidationError");
    }
    comments.push_back(comment);
}

void Post::removeComment(Comment* comment) {
    auto it = std::find(comments.begin(), comments.end(), comment);
    if (it != comments.end()) {
        comments.erase(it);
    }
}

void Post::addReaction(User* user, int reactionType) {
    if (!user) {
        throw FacebookException("Cannot add reaction from null user", "ValidationError");
    }
    reactions[user] = reactionType;
}

void Post::removeReaction(User* user) {
    reactions.erase(user);
}

int Post::getReactionType(const User* user) const {
    auto it = reactions.find(const_cast<User*>(user));
    return it != reactions.end() ? it->second : 0;
}

std::string Post::serialize() const {
    std::stringstream ss;
    ss << id << "|"
       << content << "|"
       << static_cast<int>(privacy) << "|"
       << author->getEmail() << "|"
       << createdAt.serialize();
    return ss.str();
}

Post Post::deserialize(const std::string& json) {
    // Implementation will be added when needed
    throw FacebookException("Not implemented yet", "NotImplementedError");
}
