/**
 * @file safe_strings_test.c
 * @brief Test suite for the safe strings library
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This file contains comprehensive unit tests for the safe strings library,
 * testing all core functionality including creation, manipulation, searching,
 * formatting, and safety features.
 */

#include <assert.h>

#include "safe_strings.h"

/**
 * @brief Test function for string creation operations
 * @details Tests various string creation methods including:
 *          - Basic string creation
 *          - Creation with specific capacity
 *          - Creation from C string
 *          - Creation from buffer with explicit length
 */
void test_string_creation(void) {
    printf("Testing string creation...\n");

    // Test basic creation
    string_t *str1 = string_create();
    assert(str1 != NULL);
    assert(string_is_empty(str1));
    assert(string_length(str1) == 0);
    assert(string_capacity(str1) >= STRING_DEFAULT_CAPACITY);
    string_destroy(str1);

    // Test creation with capacity
    string_t *str2 = string_create_with_capacity(100);
    assert(str2 != NULL);
    assert(string_capacity(str2) >= 100);
    string_destroy(str2);

    // Test creation from C string
    string_t *str3 = string_create_from_cstr("Hello, World!");
    assert(str3 != NULL);
    assert(string_length(str3) == 13);
    assert(string_equals_cstr(str3, "Hello, World!"));
    string_destroy(str3);

    // Test creation from buffer
    const char buffer[] = "Test\0Hidden";
    string_t *str4 = string_create_from_buffer(buffer, 4);
    assert(str4 != NULL);
    assert(string_length(str4) == 4);
    assert(string_equals_cstr(str4, "Test"));
    string_destroy(str4);

    printf("✅ String creation tests passed\n");
}

/**
 * @brief Test function for string assignment operations
 * @details Tests various string assignment methods including:
 *          - C string assignment
 *          - Buffer assignment with explicit length
 *          - String-to-string assignment
 */
void test_string_assignment(void) {
    printf("Testing string assignment...\n");

    string_t *str = string_create();

    // Test C string assignment
    assert(string_assign_cstr(str, "Hello") == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello"));
    assert(string_length(str) == 5);

    // Test buffer assignment
    const char buffer[] = "World!";
    assert(string_assign_buffer(str, buffer, 5) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "World"));
    assert(string_length(str) == 5);

    // Test string assignment
    string_t *str2 = string_create_from_cstr("Testing");
    assert(string_assign_string(str, str2) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Testing"));

    string_destroy(str);
    string_destroy(str2);

    printf("✅ String assignment tests passed\n");
}

/**
 * @brief Test function for string concatenation operations
 * @details Tests various string concatenation methods including:
 *          - C string append
 *          - Character append
 *          - String-to-string append
 */
void test_string_concatenation(void) {
    printf("Testing string concatenation...\n");

    string_t *str = string_create_from_cstr("Hello");

    // Test C string append
    assert(string_append_cstr(str, ", ") == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, "));

    // Test character append
    assert(string_append_char(str, 'W') == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, W"));

    // Test string append
    string_t *str2 = string_create_from_cstr("orld!");
    assert(string_append_string(str, str2) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, World!"));

    string_destroy(str);
    string_destroy(str2);

    printf("✅ String concatenation tests passed\n");
}

/**
 * @brief Test function for string insertion operations
 * @details Tests string insertion functionality including:
 *          - Character insertion at specific position
 *          - C string insertion at specific position
 */
void test_string_insertion(void) {
    printf("Testing string insertion...\n");

    string_t *str = string_create_from_cstr("Hello World!");

    // Test character insertion
    assert(string_insert_char(str, 5, ',') == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, World!"));

    // Test C string insertion
    assert(string_insert_cstr(str, 7, "Beautiful ") == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, Beautiful World!"));

    string_destroy(str);

    printf("✅ String insertion tests passed\n");
}

/**
 * @brief Test function for string removal operations
 * @details Tests string removal functionality including:
 *          - Erase substring from specific position
 *          - Pop back character removal
 */
void test_string_removal(void) {
    printf("Testing string removal...\n");

    string_t *str = string_create_from_cstr("Hello, Beautiful World!");

    // Test erase
    assert(string_erase(str, 7, 10) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, World!"));

    // Test pop back
    assert(string_pop_back(str) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, World"));

    string_destroy(str);

    printf("✅ String removal tests passed\n");
}

/**
 * @brief Test function for string searching operations
 * @details Tests string searching functionality including:
 *          - Character search with forward direction
 *          - Substring search in string
 *          - Reverse character search
 */
void test_string_searching(void) {
    printf("Testing string searching...\n");

    string_t *str = string_create_from_cstr("Hello, World! Hello again!");

    // Test character search
    assert(string_find_char(str, 'o', 0) == 4);
    assert(string_find_char(str, 'o', 5) == 8);
    assert(string_find_char(str, 'x', 0) == STRING_NPOS);

    // Test substring search
    assert(string_find_cstr(str, "World", 0) == 7);
    assert(string_find_cstr(str, "Hello", 1) == 14);
    assert(string_find_cstr(str, "xyz", 0) == STRING_NPOS);

    // Test reverse search
    assert(string_rfind_char(str, 'o', STRING_NPOS) == 18);

    string_destroy(str);

    printf("✅ String searching tests passed\n");
}

/**
 * @brief Test function for string utility operations
 * @details Tests string utility functionality including:
 *          - Case conversion (upper/lower)
 *          - String trimming
 *          - Character replacement
 */
void test_string_utility(void) {
    printf("Testing string utility functions...\n");

    // Test case conversion
    string_t *str = string_create_from_cstr("Hello, World!");
    assert(string_to_upper(str) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "HELLO, WORLD!"));

    assert(string_to_lower(str) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "hello, world!"));

    // Test trim
    string_assign_cstr(str, "  \t Hello, World! \n ");
    assert(string_trim(str) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hello, World!"));

    // Test character replacement
    assert(string_replace_char(str, 'o', '0') == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Hell0, W0rld!"));

    string_destroy(str);

    printf("✅ String utility tests passed\n");
}

/**
 * @brief Test function for string formatting operations
 * @details Tests string formatting functionality including:
 *          - String format with printf-style formatting
 *          - String append format operation
 */
void test_string_formatting(void) {
    printf("Testing string formatting...\n");

    string_t *str = string_create();

    // Test format
    assert(string_format(str, "Number: %d, String: %s", 42, "test") == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Number: 42, String: test"));

    // Test append format
    assert(string_append_format(str, " - %s: %.2f", "Pi", 3.14159) == STRING_SUCCESS);
    assert(string_equals_cstr(str, "Number: 42, String: test - Pi: 3.14"));

    string_destroy(str);

    printf("✅ String formatting tests passed\n");
}

/**
 * @brief Test function for string safety features
 * @details Tests string safety functionality including:
 *          - Null pointer handling
 *          - Buffer overflow protection
 *          - Safe copy operations
 */
void test_string_safety(void) {
    printf("Testing string safety features...\n");

    // Test null pointer handling
    assert(string_length(NULL) == 0);
    assert(string_is_empty(NULL) == true);
    assert(string_cstr(NULL)[0] == '\0');

    // Test buffer overflow protection
    string_t *str = string_create_from_cstr("Test");
    char buffer[3];
    assert(string_copy_to_buffer(str, buffer, sizeof(buffer)) == STRING_ERROR_BUFFER_TOO_SMALL);
    assert(buffer[0] == 'T');
    assert(buffer[1] == 'e');
    assert(buffer[2] == '\0');

    string_destroy(str);

    printf("✅ String safety tests passed\n");
}

/**
 * @brief Main test runner function
 * @details Executes all test suites for the safe strings library including:
 *          - String creation tests
 *          - String assignment tests
 *          - String concatenation tests
 *          - String insertion tests
 *          - String removal tests
 *          - String searching tests
 *          - String utility tests
 *          - String formatting tests
 *          - String safety tests
 * @return int Returns 0 on successful completion of all tests
 */
int main(void) {
    printf("Running strings safety library tests...\n\n");

    test_string_creation();
    test_string_assignment();
    test_string_concatenation();
    test_string_insertion();
    test_string_removal();
    test_string_searching();
    test_string_utility();
    test_string_formatting();
    test_string_safety();

    printf("\n🎉 All tests passed! The strings library is working correctly.\n");

    return 0;
}
