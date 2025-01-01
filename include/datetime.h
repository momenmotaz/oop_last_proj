#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <sstream>
#include <iomanip>

class DateTime {
private:
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;  // Added seconds
    
    bool isLeapYear(int year) const;
    int getDaysInMonth(int month, int year) const;

public:
    // Constructors
    DateTime(int day, int month, int year)
        : day(day), month(month), year(year), hour(0), minute(0), second(0) {}
        
    DateTime(int day, int month, int year, int hour, int minute)
        : day(day), month(month), year(year), hour(hour), minute(minute), second(0) {}
        
    DateTime(int day, int month, int year, int hour, int minute, int second)
        : day(day), month(month), year(year), hour(hour), minute(minute), second(second) {}
    
    // Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getHour() const;
    int getMinute() const;
    int getSecond() const;  // Added getter for seconds
    
    // Validation
    bool isValid() const;
    
    // String representation
    std::string toString() const;
    
    // Comparison operators
    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator==(const DateTime& other) const;
};

#endif // DATETIME_H
