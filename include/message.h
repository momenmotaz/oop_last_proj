#ifndef MESSAGE_H
#define MESSAGE_H

#include "datetime.h"
#include "facebook_exception.h"
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>

class Message {
private:
    int senderId;
    int receiverId;
    std::string content;
    DateTime timestamp;
    bool read;

    DateTime getCurrentTime() const {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::tm* ltm = std::localtime(&time);
        
        // Get milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count() % 1000;
        
        return DateTime(
            ltm->tm_mday,
            1 + ltm->tm_mon,
            1900 + ltm->tm_year,
            ltm->tm_hour,
            ltm->tm_min,
            ltm->tm_sec
        );
    }

    void validate() const {
        if (content.empty()) {
            throw FacebookException("Message content cannot be empty", "ValidationError");
        }
        if (senderId <= 0 || receiverId <= 0) {
            throw FacebookException("Invalid user ID", "ValidationError");
        }
    }

public:
    Message(int sender, int receiver, const std::string& msg)
        : senderId(sender), receiverId(receiver), content(msg), 
          timestamp(getCurrentTime()), read(false) {
        validate();
    }

    // Getters
    int getSenderId() const { return senderId; }
    int getReceiverId() const { return receiverId; }
    const std::string& getContent() const { return content; }
    const DateTime& getTimestamp() const { return timestamp; }
    bool isRead() const { return read; }

    // Mark message as read
    void markAsRead() { read = true; }

    // String representation
    std::string toString() const {
        std::stringstream ss;
        ss << "From: " << senderId << "\n"
           << "To: " << receiverId << "\n"
           << "Time: " << timestamp.toString() << "\n"
           << "Status: " << (read ? "Read" : "Unread") << "\n"
           << "Content: " << content;
        return ss.str();
    }

    // Comparison operators (based on timestamp)
    bool operator<(const Message& other) const {
        return timestamp < other.timestamp;
    }

    bool operator>(const Message& other) const {
        return timestamp > other.timestamp;
    }

    bool operator==(const Message& other) const {
        return timestamp == other.timestamp &&
               senderId == other.senderId &&
               receiverId == other.receiverId &&
               content == other.content;
    }
};

#endif // MESSAGE_H
