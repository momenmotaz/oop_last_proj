#include "../include/conversation.h"
#include <sstream>

// Initialize static member
int Conversation::nextId = 1;

Conversation::Conversation(const std::vector<int>& participants)
    : id(nextId++), participants(participants)
{
    if (!isValid()) {
        throw FacebookException("Invalid conversation parameters", "ValidationError");
    }
}

bool Conversation::isValidParticipant(int userId) const {
    return userId > 0;
}

bool Conversation::hasValidParticipants() const {
    return std::all_of(participants.begin(), participants.end(),
                      [this](int userId) { return isValidParticipant(userId); });
}

bool Conversation::hasDuplicateParticipants() const {
    std::unordered_set<int> uniqueParticipants(participants.begin(), participants.end());
    return uniqueParticipants.size() != participants.size();
}

void Conversation::addMessage(const std::shared_ptr<Message>& message) {
    if (!message) {
        throw FacebookException("Message cannot be null", "ValidationError");
    }
    
    // Verify sender and receiver are participants
    if (!isParticipant(message->getSenderId()) || !isParticipant(message->getReceiverId())) {
        throw FacebookException("Message sender or receiver is not a participant", "ValidationError");
    }
    
    messages.push_back(message);
}

std::vector<std::shared_ptr<Message>> Conversation::getMessagesByUser(int userId) const {
    std::vector<std::shared_ptr<Message>> userMessages;
    
    std::copy_if(messages.begin(), messages.end(), std::back_inserter(userMessages),
                 [userId](const auto& msg) { return msg->getSenderId() == userId; });
    
    return userMessages;
}

std::vector<std::shared_ptr<Message>> Conversation::getUnreadMessages(int userId) const {
    std::vector<std::shared_ptr<Message>> unreadMessages;
    
    std::copy_if(messages.begin(), messages.end(), std::back_inserter(unreadMessages),
                 [userId](const auto& msg) { 
                     return msg->getReceiverId() == userId && !msg->isRead(); 
                 });
    
    return unreadMessages;
}

void Conversation::addParticipant(int userId) {
    if (!isValidParticipant(userId)) {
        throw FacebookException("Invalid user ID", "ValidationError");
    }
    
    if (isParticipant(userId)) {
        throw FacebookException("User is already a participant", "ValidationError");
    }
    
    participants.push_back(userId);
}

void Conversation::removeParticipant(int userId) {
    auto it = std::find(participants.begin(), participants.end(), userId);
    if (it != participants.end()) {
        participants.erase(it);
    }
}

bool Conversation::isParticipant(int userId) const {
    return std::find(participants.begin(), participants.end(), userId) != participants.end();
}

bool Conversation::isValid() const {
    return !participants.empty() && hasValidParticipants() && !hasDuplicateParticipants();
}

std::string Conversation::toString() const {
    std::stringstream ss;
    ss << "Conversation #" << id << "\n"
       << "Participants: ";
    
    for (int userId : participants) {
        ss << userId << " ";
    }
    ss << "\n\nMessages:\n";
    
    for (const auto& msg : messages) {
        ss << "---\n" << msg->toString() << "\n";
    }
    
    return ss.str();
}
