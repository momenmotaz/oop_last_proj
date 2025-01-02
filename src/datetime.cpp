#include "../include/datetime.h"
#include <sstream>
#include <iomanip>

int DateTime::getDay() const { return day; }
int DateTime::getMonth() const { return month; }
int DateTime::getYear() const { return year; }
int DateTime::getHour() const { return hour; }
int DateTime::getMinute() const { return minute; }
int DateTime::getSecond() const { return second; }

bool DateTime::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int DateTime::getDaysInMonth(int month, int year) const {
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return daysInMonth[month - 1];
}

bool DateTime::isValid() const {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(month, year)) return false;
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    if (second < 0 || second > 59) return false;
    return true;
}

std::string DateTime::toString() const {
    return serialize();
}

std::string DateTime::serialize() const {
    std::ostringstream ss;
    ss << year << '-'
       << std::setw(2) << std::setfill('0') << month << '-'
       << std::setw(2) << std::setfill('0') << day << ' '
       << std::setw(2) << std::setfill('0') << hour << ':'
       << std::setw(2) << std::setfill('0') << minute << ':'
       << std::setw(2) << std::setfill('0') << second;
    return ss.str();
}

DateTime DateTime::deserialize(const std::string& data) {
    std::istringstream ss(data);
    char delimiter;
    int year, month, day, hour = 0, minute = 0, second = 0;
    
    ss >> year >> delimiter >> month >> delimiter >> day;
    
    if (ss >> delimiter) {  // If there's more data (time)
        ss >> hour >> delimiter >> minute >> delimiter >> second;
    }
    
    return DateTime(day, month, year, hour, minute, second);
}

bool DateTime::operator<(const DateTime& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    if (minute != other.minute) return minute < other.minute;
    return second < other.second;
}

bool DateTime::operator>(const DateTime& other) const {
    return other < *this;
}

bool DateTime::operator==(const DateTime& other) const {
    return !(*this < other) && !(other < *this);
}
