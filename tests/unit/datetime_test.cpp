#include "../../include/datetime.h"
#include <cassert>
#include <iostream>

void testDateTimeCreation() {
    std::cout << "Testing DateTime Creation..." << std::endl;
    
    // Test 1: Valid date
    DateTime dt1(1, 1, 2024);
    assert(dt1.isValid() && "Test 1.1 failed: Valid date should be valid");
    assert(dt1.getDay() == 1 && "Test 1.2 failed: Day mismatch");
    assert(dt1.getMonth() == 1 && "Test 1.3 failed: Month mismatch");
    assert(dt1.getYear() == 2024 && "Test 1.4 failed: Year mismatch");
    std::cout << "Test 1 passed: Valid date creation" << std::endl;
    
    // Test 2: Valid date with time
    DateTime dt2(15, 3, 2024, 14, 30, 45);
    assert(dt2.isValid() && "Test 2.1 failed: Valid date with time should be valid");
    assert(dt2.getHour() == 14 && "Test 2.2 failed: Hour mismatch");
    assert(dt2.getMinute() == 30 && "Test 2.3 failed: Minute mismatch");
    assert(dt2.getSecond() == 45 && "Test 2.4 failed: Second mismatch");
    std::cout << "Test 2 passed: Valid date with time creation" << std::endl;
}

void testDateTimeValidation() {
    std::cout << "\nTesting DateTime Validation..." << std::endl;
    
    // Test 3: Invalid dates
    DateTime dt3(31, 2, 2024);  // February can't have 31 days
    assert(!dt3.isValid() && "Test 3.1 failed: Invalid date should not be valid");
    
    DateTime dt4(29, 2, 2023);  // 2023 is not a leap year
    assert(!dt4.isValid() && "Test 3.2 failed: Feb 29 in non-leap year should not be valid");
    
    DateTime dt5(29, 2, 2024);  // 2024 is a leap year
    assert(dt5.isValid() && "Test 3.3 failed: Feb 29 in leap year should be valid");
    std::cout << "Test 3 passed: Date validation" << std::endl;
}

void testDateTimeComparison() {
    std::cout << "\nTesting DateTime Comparison..." << std::endl;
    
    // Test 4: Date comparison
    DateTime dt1(1, 1, 2024);
    DateTime dt2(2, 1, 2024);
    DateTime dt3(1, 1, 2024, 0, 0, 1);
    
    assert(dt1 < dt2 && "Test 4.1 failed: Earlier date should be less than later date");
    assert(dt1 < dt3 && "Test 4.2 failed: Same date with earlier time should be less");
    assert(dt2 > dt1 && "Test 4.3 failed: Later date should be greater than earlier date");
    assert(DateTime(1, 1, 2024) == DateTime(1, 1, 2024) && "Test 4.4 failed: Equal dates should be equal");
    std::cout << "Test 4 passed: Date comparison" << std::endl;
}

void testDateTimeFormatting() {
    std::cout << "\nTesting DateTime Formatting..." << std::endl;
    
    // Test 5: String representation
    DateTime dt1(15, 3, 2024);
    assert(dt1.toString() == "2024-03-15" && "Test 5.1 failed: Date format mismatch");
    
    DateTime dt2(15, 3, 2024, 14, 30, 45);
    assert(dt2.toString() == "2024-03-15 14:30:45" && "Test 5.2 failed: Date-time format mismatch");
    std::cout << "Test 5 passed: Date formatting" << std::endl;
}

int main() {
    try {
        testDateTimeCreation();
        testDateTimeValidation();
        testDateTimeComparison();
        testDateTimeFormatting();
        std::cout << "\nAll DateTime unit tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
