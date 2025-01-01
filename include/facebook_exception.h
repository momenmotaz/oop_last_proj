#ifndef FACEBOOK_EXCEPTION_H
#define FACEBOOK_EXCEPTION_H

#include <string>
#include <exception>
#include <sstream>

class FacebookException : public std::exception {
private:
    std::string message;
    std::string type;
    mutable std::string formatted_message;

    void updateFormattedMessage() {
        std::stringstream ss;
        ss << type << ": " << message;
        formatted_message = ss.str();
    }

public:
    // Constructor
    FacebookException(const std::string& msg, const std::string& err_type)
        : message(msg), type(err_type) {
        updateFormattedMessage();
    }
    
    // Copy constructor
    FacebookException(const FacebookException& other)
        : message(other.message), type(other.type), formatted_message(other.formatted_message) {}
    
    // Assignment operator
    FacebookException& operator=(const FacebookException& other) {
        if (this != &other) {
            message = other.message;
            type = other.type;
            formatted_message = other.formatted_message;
        }
        return *this;
    }
    
    // Override what() from std::exception
    const char* what() const noexcept override {
        return formatted_message.c_str();
    }
    
    // Get the type of error
    const std::string& getType() const {
        return type;
    }
    
    // Format message with details
    template<typename... Args>
    void setDetails(Args... args) {
        std::stringstream ss;
        formatMessage(ss, message.c_str(), std::forward<Args>(args)...);
        message = ss.str();
        updateFormattedMessage();
    }

private:
    // Base case for formatting
    void formatMessage(std::stringstream& ss, const char* format) {
        ss << format;
    }

    // Recursive case for formatting
    template<typename T, typename... Args>
    void formatMessage(std::stringstream& ss, const char* format, T value, Args... args) {
        while (*format) {
            if (*format == '%') {
                ++format;
                if (*format == 'd' || *format == 's') {
                    ss << value;
                    formatMessage(ss, format + 1, args...);
                    return;
                }
            }
            ss << *format++;
        }
    }
};

#endif // FACEBOOK_EXCEPTION_H
