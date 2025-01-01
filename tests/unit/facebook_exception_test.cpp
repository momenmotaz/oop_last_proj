#include "../../include/facebook_exception.h"
#include <cassert>
#include <iostream>
#include <string>

void testExceptionCreation() {
    std::cout << "Testing FacebookException Creation..." << std::endl;
    
    // Test 1: Basic exception creation
    FacebookException ex1("Test message", "TestError");
    std::string expected = "TestError: Test message";
    std::string actual = ex1.what();
    assert(actual == expected && 
           "Test 1.1 failed: Exception message format mismatch");
    assert(ex1.getType() == "TestError" && 
           "Test 1.2 failed: Exception type mismatch");
    std::cout << "Test 1 passed: Basic exception creation" << std::endl;
}

void testExceptionTypes() {
    std::cout << "\nTesting Different Exception Types..." << std::endl;
    
    // Test 2: Different types of exceptions
    FacebookException ex1("Invalid input", "ValidationError");
    FacebookException ex2("Not found", "NotFoundError");
    FacebookException ex3("Access denied", "SecurityError");
    
    assert(ex1.getType() == "ValidationError" && 
           "Test 2.1 failed: ValidationError type mismatch");
    assert(ex2.getType() == "NotFoundError" && 
           "Test 2.2 failed: NotFoundError type mismatch");
    assert(ex3.getType() == "SecurityError" && 
           "Test 2.3 failed: SecurityError type mismatch");
    std::cout << "Test 2 passed: Exception types" << std::endl;
}

void testExceptionMessageFormatting() {
    std::cout << "\nTesting Exception Message Formatting..." << std::endl;
    
    // Test 3: Message formatting
    FacebookException ex("Custom message", "CustomError");
    std::string expected = "CustomError: Custom message";
    std::string actual = ex.what();
    assert(actual == expected && 
           "Test 3 failed: Exception message format mismatch");
    std::cout << "Test 3 passed: Message formatting" << std::endl;
}

void testExceptionInheritance() {
    std::cout << "\nTesting Exception Inheritance..." << std::endl;
    
    // Test 4: Exception inheritance
    FacebookException ex("Test message", "TestError");
    bool isStdException = dynamic_cast<const std::exception*>(&ex) != nullptr;
    assert(isStdException && "Test 4 failed: Should inherit from std::exception");
    std::cout << "Test 4 passed: Exception inheritance" << std::endl;
}

int main() {
    try {
        testExceptionCreation();
        testExceptionTypes();
        testExceptionMessageFormatting();
        testExceptionInheritance();
        std::cout << "\nAll FacebookException unit tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
