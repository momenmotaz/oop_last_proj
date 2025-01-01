#include "../include/message.h"
#include <sstream>

Message::Message(int senderId, int receiverId, const std::string& content)
    : senderId(senderId), receiverId(receiverId), content(content),
      timestamp(DateTime(2025, 1, 2, 1, 22, 52)), read(false)  // Current time from context
{
    if (!isValid()) {
        throw FacebookException("Invalid message parameters", "ValidationError");
    }
}

bool Message::isValid() const {
    return senderId > 0 && receiverId > 0 && !content.empty();
}

std::string Message::toString() const {
    std::stringstream ss;
    ss << "From: User #" << senderId << "\n"
       << "To: User #" << receiverId << "\n"
       << "Content: " << content << "\n"
       << "Time: " << timestamp.toString() << "\n"
       << "Status: " << (read ? "Read" : "Unread");
    return ss.str();
}
