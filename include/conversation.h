#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "facebook_exception.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <algorithm>

template<typename MessageType>
class Conversation {
private:
    int id;
    std::vector<int> participants;
    std::vector<std::shared_ptr<MessageType>> messages;
    static int nextId;

    // Validation helpers
    bool isValidParticipant(int userId) const { return userId > 0; }
    bool hasValidParticipants() const {
        return std::all_of(participants.begin(), participants.end(),
                          [this](int userId) { return isValidParticipant(userId); });
    }
    bool hasDuplicateParticipants() const {
        std::unordered_set<int> uniqueParticipants(participants.begin(), participants.end());
        return uniqueParticipants.size() != participants.size();
    }

    // Sort messages by timestamp
    void sortMessages() {
        std::sort(messages.begin(), messages.end(),
                 [](const auto& a, const auto& b) { return a->getTimestamp() < b->getTimestamp(); });
    }

public:
    // Constructor
    explicit Conversation(const std::vector<int>& participants)
        : id(nextId++), participants(participants)
    {
        if (!hasValidParticipants() || hasDuplicateParticipants()) {
            throw FacebookException("Invalid conversation parameters", "ValidationError");
        }
    }
    
    // Getters
    int getId() const { return id; }
    const std::vector<int>& getParticipants() const { return participants; }
    const std::vector<std::shared_ptr<MessageType>>& getMessages() const { return messages; }
    
    // Message management
    void addMessage(const std::shared_ptr<MessageType>& message) {
        if (!message) {
            throw FacebookException("Message cannot be null", "ValidationError");
        }
        if (!isParticipant(message->getSenderId()) || !isParticipant(message->getReceiverId())) {
            throw FacebookException("Message sender or receiver is not a participant", "ValidationError");
        }
        messages.push_back(message);
        sortMessages();  // Sort messages after adding new one
    }
    
    std::vector<std::shared_ptr<MessageType>> getMessagesByUser(int userId) const {
        std::vector<std::shared_ptr<MessageType>> userMessages;
        std::copy_if(messages.begin(), messages.end(), std::back_inserter(userMessages),
                     [userId](const auto& msg) { return msg->getSenderId() == userId; });
        return userMessages;
    }
    
    std::vector<std::shared_ptr<MessageType>> getUnreadMessages(int userId) const {
        std::vector<std::shared_ptr<MessageType>> unreadMessages;
        std::copy_if(messages.begin(), messages.end(), std::back_inserter(unreadMessages),
                     [userId](const auto& msg) { 
                         return msg->getReceiverId() == userId && !msg->isRead(); 
                     });
        return unreadMessages;
    }
    
    // Participant management
    void addParticipant(int userId) {
        if (!isValidParticipant(userId)) {
            throw FacebookException("Invalid user ID", "ValidationError");
        }
        if (isParticipant(userId)) {
            throw FacebookException("User is already a participant", "ValidationError");
        }
        participants.push_back(userId);
    }
    
    void removeParticipant(int userId) {
        auto it = std::find(participants.begin(), participants.end(), userId);
        if (it != participants.end()) {
            participants.erase(it);
        }
    }
    
    bool isParticipant(int userId) const {
        return std::find(participants.begin(), participants.end(), userId) != participants.end();
    }
    
    // Comparison operators
    bool operator<(const Conversation<MessageType>& other) const { return id < other.id; }
    bool operator>(const Conversation<MessageType>& other) const { return id > other.id; }
    bool operator==(const Conversation<MessageType>& other) const { return id == other.id; }
};

// Initialize static member
template<typename MessageType>
int Conversation<MessageType>::nextId = 1;

#endif // CONVERSATION_H
