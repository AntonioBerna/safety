/**
 * @file sstring.h
 * @brief Safe and dynamic string library for C
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This library provides a safe, dynamic string implementation for C that prevents
 * common string-related security vulnerabilities such as buffer overflows and
 * use-after-free errors. It offers automatic memory management, bounds checking,
 * and a comprehensive API for string manipulation.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/** @brief Default initial capacity for dynamic strings */
#define STRING_DEFAULT_CAPACITY 64

/** @brief Growth factor for dynamic string expansion */
#define STRING_GROWTH_FACTOR 2

/** @brief Constant representing "not found" position */
#define STRING_NPOS ((size_t)-1)

/**
 * @brief Error codes for string operations
 */
typedef enum {
    STRING_SUCCESS                =  0,  /**< Operation completed successfully */
    STRING_ERROR_NULL_POINTER     = -1,  /**< Null pointer was passed */
    STRING_ERROR_OUT_OF_MEMORY    = -2,  /**< Memory allocation failed */
    STRING_ERROR_INVALID_INDEX    = -3,  /**< Index is out of bounds */
    STRING_ERROR_BUFFER_TOO_SMALL = -4,  /**< Provided buffer is too small */
    STRING_ERROR_INVALID_ARGUMENT = -5   /**< Invalid argument provided */
} string_result_t;

/**
 * @brief Safe string structure
 */
typedef struct {
    char *data;           /**< Pointer to the string data */
    size_t length;        /**< Current length (excluding null terminator) */
    size_t capacity;      /**< Total allocated capacity */
    bool is_owner;        /**< Whether this string owns the memory */
} string_t;

/**
 * @brief Get error message for a given error code
 * @param error The error code to get message for
 * @return Human-readable error message string
 */
const char *string_error_message(string_result_t error);

/*
 * ==========================================
 * String creation and destruction functions
 * ==========================================
 */

/**
 * @brief Create a new string with specified capacity
 * @param capacity Initial capacity for the string (minimum capacity will be applied if too small)
 * @return Pointer to newly created string, or NULL on failure
 */
string_t *string_create_with_capacity(size_t capacity);

/**
 * @brief Create a new empty string with default capacity
 * @return Pointer to newly created string, or NULL on failure
 */
string_t *string_create(void);

/**
 * @brief Create a new string from a buffer with specified length
 * @param buffer Source buffer (can contain null bytes)
 * @param length Number of bytes to copy from buffer
 * @return Pointer to newly created string, or NULL on failure
 */
string_t *string_create_from_buffer(const char *buffer, size_t length);

/**
 * @brief Create a new string from a null-terminated C string
 * @param cstr Source C string (null-terminated)
 * @return Pointer to newly created string, or NULL on failure
 */
string_t *string_create_from_cstr(const char *cstr);

/**
 * @brief Create a copy of an existing string
 * @param src Source string to clone
 * @return Pointer to newly created string copy, or NULL on failure
 */
string_t *string_clone(const string_t *src);

/**
 * @brief Destroy a string and free its memory
 * @param str String to destroy (can be NULL)
 */
void string_destroy(string_t *str);

/*
 * =============================
 * String information functions
 * =============================
 */

/**
 * @brief Get the length of a string
 * @param str String to get length of
 * @return Length of the string (0 if str is NULL)
 */
size_t string_length(const string_t *str);

/**
 * @brief Get the capacity of a string
 * @param str String to get capacity of
 * @return Capacity of the string (0 if str is NULL)
 */
size_t string_capacity(const string_t *str);

/**
 * @brief Check if a string is empty
 * @param str String to check
 * @return true if string is empty or NULL, false otherwise
 */
bool string_is_empty(const string_t *str);

/**
 * @brief Get a pointer to the underlying C string
 * @param str String to get C string from
 * @return Pointer to null-terminated C string (empty string if str is NULL)
 */
const char *string_cstr(const string_t *str);

/*
 * ==============================
 * String modification functions
 * ==============================
 */

/**
 * @brief Reserve capacity for a string
 * @param str String to reserve capacity for
 * @param new_capacity Minimum capacity to reserve
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_reserve(string_t *str, size_t new_capacity);

/**
 * @brief Resize a string to specified length
 * @param str String to resize
 * @param new_length New length for the string
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_resize(string_t *str, size_t new_length);

/**
 * @brief Clear a string (set length to 0)
 * @param str String to clear
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_clear(string_t *str);

/**
 * @brief Shrink string capacity to fit current length
 * @param str String to shrink
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_shrink_to_fit(string_t *str);

/*
 * ============================
 * String assignment functions
 * ============================
 */

/**
 * @brief Assign content from buffer to string
 * @param str Destination string
 * @param buffer Source buffer
 * @param length Length of data to assign
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_assign_buffer(string_t *str, const char *buffer, size_t length);

/**
 * @brief Assign content from C string to string
 * @param str Destination string
 * @param cstr Source C string (null-terminated)
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_assign_cstr(string_t *str, const char *cstr);

/**
 * @brief Assign content from one string to another
 * @param dest Destination string
 * @param src Source string
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_assign_string(string_t *dest, const string_t *src);

/*
 * ===============================
 * String concatenation functions
 * ===============================
 */

/**
 * @brief Append buffer content to string
 * @param str Destination string
 * @param buffer Source buffer to append
 * @param length Length of data to append
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_append_buffer(string_t *str, const char *buffer, size_t length);

/**
 * @brief Append C string to string
 * @param str Destination string
 * @param cstr Source C string to append (null-terminated)
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_append_cstr(string_t *str, const char *cstr);

/**
 * @brief Append one string to another
 * @param dest Destination string
 * @param src Source string to append
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_append_string(string_t *dest, const string_t *src);

/**
 * @brief Append a single character to string
 * @param str Destination string
 * @param c Character to append
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_append_char(string_t *str, char c);

/*
 * ===========================
 * String insertion functions
 * ===========================
 */

/**
 * @brief Insert buffer content at specified position
 * @param str Destination string
 * @param index Position to insert at
 * @param buffer Source buffer to insert
 * @param length Length of data to insert
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_insert_buffer(string_t *str, size_t index, const char *buffer, size_t length);

/**
 * @brief Insert C string at specified position
 * @param str Destination string
 * @param index Position to insert at
 * @param cstr Source C string to insert (null-terminated)
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_insert_cstr(string_t *str, size_t index, const char *cstr);

/**
 * @brief Insert one string into another at specified position
 * @param dest Destination string
 * @param index Position to insert at
 * @param src Source string to insert
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_insert_string(string_t *dest, size_t index, const string_t *src);

/**
 * @brief Insert a single character at specified position
 * @param str Destination string
 * @param index Position to insert at
 * @param c Character to insert
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_insert_char(string_t *str, size_t index, char c);

/*
 * =========================
 * String removal functions
 * =========================
 */

/**
 * @brief Erase characters from string at specified position
 * @param str String to modify
 * @param index Starting position to erase from
 * @param count Number of characters to erase
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_erase(string_t *str, size_t index, size_t count);

/**
 * @brief Remove the last character from string
 * @param str String to modify
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_pop_back(string_t *str);

/*
 * ========================
 * String access functions
 * ========================
 */

/**
 * @brief Get character at specified position
 * @param str String to access
 * @param index Position to get character from
 * @return Character at position, or '\0' if index is invalid
 */
char string_at(const string_t *str, size_t index);

/**
 * @brief Set character at specified position
 * @param str String to modify
 * @param index Position to set character at
 * @param c Character to set
 * @return STRING_SUCCESS on success, error code on failure
 */
string_result_t string_set_at(string_t *str, size_t index, char c);

/*
 * ============================
 * String comparison functions
 * ============================
 */

/**
 * @brief Compare two strings lexicographically
 * @param str1 First string to compare
 * @param str2 Second string to compare
 * @return Negative if str1 < str2, 0 if equal, positive if str1 > str2
 */
int string_compare(const string_t *str1, const string_t *str2);

/**
 * @brief Compare string with C string lexicographically
 * @param str String to compare
 * @param cstr C string to compare with
 * @return Negative if str < cstr, 0 if equal, positive if str > cstr
 */
int string_compare_cstr(const string_t *str, const char *cstr);

/**
 * @brief Check if two strings are equal
 * @param str1 First string to compare
 * @param str2 Second string to compare
 * @return true if strings are equal, false otherwise
 */
bool string_equals(const string_t *str1, const string_t *str2);

/**
 * @brief Check if string equals C string
 * @param str String to compare
 * @param cstr C string to compare with
 * @return true if strings are equal, false otherwise
 */
bool string_equals_cstr(const string_t *str, const char *cstr);

/*
 * ===========================
 * String searching functions
 * ===========================
 */

/**
 * @brief Find first occurrence of character in string
 * @param str String to search in
 * @param c Character to search for
 * @param start_pos Position to start searching from (0-based)
 * @return Position of first occurrence, or STRING_NPOS if not found
 * @details Searches for the first occurrence of character c starting from start_pos.
 *          Returns STRING_NPOS if the character is not found or if parameters are invalid.
 */
size_t string_find_char(const string_t *str, char c, size_t start_pos);

/**
 * @brief Find first occurrence of C string in string
 * @param str String to search in
 * @param substr C string to search for (null-terminated)
 * @param start_pos Position to start searching from (0-based)
 * @return Position of first occurrence, or STRING_NPOS if not found
 * @details Searches for the first occurrence of the substring starting from start_pos.
 *          Returns STRING_NPOS if the substring is not found or if parameters are invalid.
 */
size_t string_find_cstr(const string_t *str, const char *substr, size_t start_pos);

/**
 * @brief Find first occurrence of string in string
 * @param str String to search in
 * @param substr String to search for
 * @param start_pos Position to start searching from (0-based)
 * @return Position of first occurrence, or STRING_NPOS if not found
 * @details Searches for the first occurrence of the substring starting from start_pos.
 *          Returns STRING_NPOS if the substring is not found or if parameters are invalid.
 */
size_t string_find_string(const string_t *str, const string_t *substr, size_t start_pos);

/**
 * @brief Find last occurrence of character in string
 * @param str String to search in
 * @param c Character to search for
 * @param start_pos Position to start searching backwards from (0-based)
 * @return Position of last occurrence, or STRING_NPOS if not found
 * @details Searches backwards for the last occurrence of character c starting from start_pos.
 *          If start_pos is STRING_NPOS, searches from the end of the string.
 *          Returns STRING_NPOS if the character is not found or if parameters are invalid.
 */
size_t string_rfind_char(const string_t *str, char c, size_t start_pos);

/*
 * =========================
 * String utility functions
 * =========================
 */

/**
 * @brief Convert string to uppercase
 * @param str String to convert to uppercase
 * @return STRING_SUCCESS on success, error code on failure
 * @details Converts all lowercase letters in the string to uppercase in-place.
 *          Non-alphabetic characters remain unchanged.
 */
string_result_t string_to_upper(string_t *str);

/**
 * @brief Convert string to lowercase
 * @param str String to convert to lowercase
 * @return STRING_SUCCESS on success, error code on failure
 * @details Converts all uppercase letters in the string to lowercase in-place.
 *          Non-alphabetic characters remain unchanged.
 */
string_result_t string_to_lower(string_t *str);

/**
 * @brief Remove leading and trailing whitespace from string
 * @param str String to trim
 * @return STRING_SUCCESS on success, error code on failure
 * @details Removes whitespace characters (space, tab, newline, etc.) from both
 *          the beginning and end of the string. The string is modified in-place.
 */
string_result_t string_trim(string_t *str);

/**
 * @brief Replace all occurrences of a character with another character
 * @param str String to modify
 * @param old_char Character to replace
 * @param new_char Character to replace with
 * @return STRING_SUCCESS on success, error code on failure
 * @details Replaces all occurrences of old_char with new_char in the string.
 *          The replacement is performed in-place.
 */
string_result_t string_replace_char(string_t *str, char old_char, char new_char);

/*
 * =========================
 * String copying functions
 * =========================
 */

/**
 * @brief Copy string content to a C-style buffer safely
 * @param str Source string to copy from
 * @param buffer Destination buffer
 * @param buffer_size Size of the destination buffer
 * @return STRING_SUCCESS on success, error code on failure
 * @details Safely copies the string content to a C-style buffer with null termination.
 *          Ensures buffer bounds are respected and returns an error if the buffer
 *          is too small to hold the string plus null terminator.
 */
string_result_t string_copy_to_buffer(const string_t *str, char *buffer, size_t buffer_size);

/*
 * ============================
 * String formatting functions
 * ============================
 */

/**
 * @brief Format string using printf-style formatting
 * @param str Destination string to store formatted result
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 * @return STRING_SUCCESS on success, error code on failure
 * @details Formats the string using printf-style formatting and stores the result
 *          in the destination string. The previous content of the string is replaced.
 *          Supports all standard printf format specifiers.
 */
string_result_t string_format(string_t *str, const char *format, ...);

/**
 * @brief Append formatted text to string using printf-style formatting
 * @param str Destination string to append to
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 * @return STRING_SUCCESS on success, error code on failure
 * @details Formats text using printf-style formatting and appends it to the
 *          existing string content. Supports all standard printf format specifiers.
 */
string_result_t string_append_format(string_t *str, const char *format, ...);
