/**
 * @file full_example.c
 * @brief Comprehensive example demonstrating safe_strings library capabilities
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This example showcases the key features of the safe_strings library,
 * including string creation, manipulation, searching, formatting, and
 * safe memory management.
 */

#include <stdio.h>
#include <stdlib.h>

#include "safe_strings.h"

/**
 * @brief Demonstrate basic string operations
 */
void demo_basic_operations(void) {
    printf("=== Basic String Operations Demo ===\n");

    // Create strings in different ways
    string_t *str1 = string_create_from_cstr("Hello, ");
    string_t *str2 = string_create_from_cstr("Safe World!");
    string_t *str3 = string_create();

    if (!str1 || !str2 || !str3) {
        fprintf(stderr, "Failed to create strings\n");
        return;
    }

    printf("Initial strings:\n");
    printf("  str1: \"%s\" (length: %zu, capacity: %zu)\n", string_cstr(str1), string_length(str1), string_capacity(str1));
    printf("  str2: \"%s\" (length: %zu, capacity: %zu)\n", string_cstr(str2), string_length(str2), string_capacity(str2));
    printf("  str3: \"%s\" (length: %zu, capacity: %zu)\n", string_cstr(str3), string_length(str3), string_capacity(str3));

    // Append operations
    string_result_t result = string_append_string(str1, str2);
    if (result == STRING_SUCCESS) {
        printf("\nAfter appending str2 to str1:\n");
        printf("  str1: \"%s\" (length: %zu)\n", string_cstr(str1), string_length(str1));
    } else {
        printf("Error appending: %s\n", string_error_message(result));
    }

    // Assignment operations
    result = string_assign_cstr(str3, "This is a new string!");
    if (result == STRING_SUCCESS) {
        printf("  str3: \"%s\" (length: %zu)\n", string_cstr(str3), string_length(str3));
    }

    // Character operations
    result = string_append_char(str3, '!');
    if (result == STRING_SUCCESS) {
        printf("After appending '!': \"%s\"\n", string_cstr(str3));
    }

    // Cleanup
    string_destroy(str1);
    string_destroy(str2);
    string_destroy(str3);
    printf("\n");
}

/**
 * @brief Demonstrate string manipulation operations
 */
void demo_manipulation(void) {
    printf("=== String Manipulation Demo ===\n");

    string_t *str = string_create_from_cstr("  Hello, SAFE Programming!  ");
    if (!str) {
        fprintf(stderr, "Failed to create string\n");
        return;
    }

    printf("Original: \"%s\"\n", string_cstr(str));

    // Trim whitespace
    string_result_t result = string_trim(str);
    if (result == STRING_SUCCESS) {
        printf("Trimmed:  \"%s\"\n", string_cstr(str));
    }

    // Convert to lowercase
    result = string_to_lower(str);
    if (result == STRING_SUCCESS) {
        printf("Lowercase: \"%s\"\n", string_cstr(str));
    }

    // Convert to uppercase
    result = string_to_upper(str);
    if (result == STRING_SUCCESS) {
        printf("Uppercase: \"%s\"\n", string_cstr(str));
    }

    // Replace characters
    result = string_replace_char(str, ' ', '_');
    if (result == STRING_SUCCESS) {
        printf("Replaced spaces: \"%s\"\n", string_cstr(str));
    }

    // Insert text
    result = string_insert_cstr(str, 6, "[VERY_");
    if (result == STRING_SUCCESS) {
        printf("After insertion: \"%s\"\n", string_cstr(str));
    }

    // Erase part of string
    result = string_erase(str, 6, 6); // Remove "[VERY_"
    if (result == STRING_SUCCESS) {
        printf("After erasure: \"%s\"\n", string_cstr(str));
    }

    string_destroy(str);
    printf("\n");
}

/**
 * @brief Demonstrate searching and comparison operations
 */
void demo_search_compare(void) {
    printf("=== Search and Comparison Demo ===\n");

    string_t *text = string_create_from_cstr("The quick brown fox jumps over the lazy dog");
    string_t *pattern = string_create_from_cstr("fox");

    if (!text || !pattern) {
        fprintf(stderr, "Failed to create strings\n");
        return;
    }

    printf("Text: \"%s\"\n", string_cstr(text));
    printf("Pattern: \"%s\"\n", string_cstr(pattern));

    // Find substring
    size_t pos = string_find_string(text, pattern, 0);
    if (pos != STRING_NPOS) {
        printf("Found \"%s\" at position %zu\n", string_cstr(pattern), pos);
    } else {
        printf("Pattern not found\n");
    }

    // Find character
    pos = string_find_char(text, 'o', 0);
    if (pos != STRING_NPOS) {
        printf("First 'o' found at position %zu\n", pos);
    }

    // Reverse find character
    pos = string_rfind_char(text, 'o', STRING_NPOS);
    if (pos != STRING_NPOS) {
        printf("Last 'o' found at position %zu\n", pos);
    }

    // String comparison
    string_t *str1 = string_create_from_cstr("apple");
    string_t *str2 = string_create_from_cstr("banana");

    if (str1 && str2) {
        int cmp = string_compare(str1, str2);
        printf("\nComparing \"%s\" and \"%s\": %d ", string_cstr(str1), string_cstr(str2), cmp);
        if (cmp < 0) {
            printf("(first < second)\n");
        } else if (cmp > 0) {
            printf("(first > second)\n");
        } else {
            printf("(equal)\n");
        }

        // Equality check
        bool equal = string_equals(str1, str2);
        printf("Are they equal? %s\n", equal ? "Yes" : "No");
    }

    string_destroy(text);
    string_destroy(pattern);
    string_destroy(str1);
    string_destroy(str2);
    printf("\n");
}

/**
 * @brief Demonstrate formatting operations
 */
void demo_formatting(void) {
    printf("=== Formatting Demo ===\n");

    string_t *str = string_create();
    if (!str) {
        fprintf(stderr, "Failed to create string\n");
        return;
    }

    // Format with integers and strings
    string_result_t result = string_format(str, "Hello %s! You have %d new messages.", "Alice", 5);
    if (result == STRING_SUCCESS) {
        printf("Formatted: \"%s\"\n", string_cstr(str));
    }

    // Append formatted content
    result = string_append_format(str, " Current time: %02d:%02d", 14, 30);
    if (result == STRING_SUCCESS) {
        printf("After append: \"%s\"\n", string_cstr(str));
    }

    // Format with floating point
    result = string_format(str, "Pi is approximately %.2f", 3.14159);
    if (result == STRING_SUCCESS) {
        printf("Pi string: \"%s\"\n", string_cstr(str));
    }

    string_destroy(str);
    printf("\n");
}

/**
 * @brief Demonstrate memory management and capacity operations
 */
void demo_memory_management(void) {
    printf("=== Memory Management Demo ===\n");

    string_t *str = string_create_with_capacity(50);
    if (!str) {
        fprintf(stderr, "Failed to create string\n");
        return;
    }

    printf("Initial capacity: %zu\n", string_capacity(str));

    // Reserve more capacity
    string_result_t result = string_reserve(str, 100);
    if (result == STRING_SUCCESS) {
        printf("After reserve(100): %zu\n", string_capacity(str));
    }

    // Add some content
    string_assign_cstr(str, "Short text");
    printf("Content: \"%s\" (length: %zu, capacity: %zu)\n", string_cstr(str), string_length(str), string_capacity(str));

    // Shrink to fit
    result = string_shrink_to_fit(str);
    if (result == STRING_SUCCESS) {
        printf("After shrink_to_fit: capacity = %zu\n", string_capacity(str));
    }

    // Resize string
    result = string_resize(str, 20);
    if (result == STRING_SUCCESS) {
        printf("After resize(20): \"%s\" (length: %zu)\n", string_cstr(str), string_length(str));
    }

    // Clear content
    result = string_clear(str);
    if (result == STRING_SUCCESS) {
        printf("After clear: \"%s\" (length: %zu, capacity: %zu)\n", string_cstr(str), string_length(str), string_capacity(str));
    }

    string_destroy(str);
    printf("\n");
}

/**
 * @brief Demonstrate safe copying to C buffers
 */
void demo_safe_copying(void) {
    printf("=== Safe Copying Demo ===\n");

    string_t *str = string_create_from_cstr("This is a long string that might not fit in a small buffer");
    if (!str) {
        fprintf(stderr, "Failed to create string\n");
        return;
    }

    printf("Source string: \"%s\" (length: %zu)\n", string_cstr(str), string_length(str));

    // Copy to small buffer
    char small_buffer[20];
    string_result_t result = string_copy_to_buffer(str, small_buffer, sizeof(small_buffer));
    printf("Copy to small buffer (size %zu): \"%s\"\n", sizeof(small_buffer), small_buffer);
    printf("Result: %s\n", string_error_message(result));

    // Copy to large buffer
    char large_buffer[100];
    result = string_copy_to_buffer(str, large_buffer, sizeof(large_buffer));
    printf("Copy to large buffer (size %zu): \"%s\"\n", sizeof(large_buffer), large_buffer);
    printf("Result: %s\n", string_error_message(result));

    string_destroy(str);
    printf("\n");
}

/**
 * @brief Main function demonstrating all safe_strings capabilities
 */
int main(void) {
    printf("Safe Strings Library - Comprehensive Example\n");
    printf("============================================\n\n");

    demo_basic_operations();
    demo_manipulation();
    demo_search_compare();
    demo_formatting();
    demo_memory_management();
    demo_safe_copying();

    printf("Example completed successfully!\n");
    printf("All operations were performed safely with automatic memory management.\n");

    return 0;
}
