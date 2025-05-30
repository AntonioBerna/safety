/**
 * @file safe_strings.c
 * @brief Implementation of the safe strings library
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This file contains the complete implementation of the safe strings library,
 * providing memory-safe string operations with automatic memory management,
 * bounds checking, and comprehensive error handling.
 */

#include "safe_strings.h"

/*
 * ==========================
 * Internal helper functions
 * ==========================
 */

/**
 * @brief Calculate the new capacity for string growth
 * @param current_capacity Current capacity of the string
 * @param required_capacity Minimum required capacity
 * @return size_t New capacity that satisfies the requirement
 * @details Uses exponential growth strategy to minimize reallocations
 */
static size_t string_calculate_growth(size_t current_capacity, size_t required_capacity) {
    size_t new_capacity = current_capacity;

    while (new_capacity < required_capacity) {
        new_capacity *= STRING_GROWTH_FACTOR;
    }

    return new_capacity;
}

/**
 * @brief Ensure string has sufficient capacity for operations
 * @param str Pointer to the string structure
 * @param required_capacity Minimum capacity needed
 * @return string_result_t Success or error code
 * @details Reallocates memory if current capacity is insufficient
 */
static string_result_t string_ensure_capacity(string_t *str, size_t required_capacity) {
    if (!str || !str->is_owner) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (str->capacity >= required_capacity) {
        return STRING_SUCCESS;
    }

    size_t new_capacity = string_calculate_growth(str->capacity, required_capacity);
    char *new_data = realloc(str->data, new_capacity);
    if (!new_data) {
        return STRING_ERROR_OUT_OF_MEMORY;
    }

    str->data = new_data;
    str->capacity = new_capacity;

    return STRING_SUCCESS;
}

/**
 * @brief Get human-readable error message for result code
 * @param error The error code to convert to message
 * @return const char* Human-readable error message
 * @details Returns appropriate error message string for each error code
 */
const char *string_error_message(string_result_t error) {
    switch (error) {
    case STRING_SUCCESS: return "Success";
    case STRING_ERROR_NULL_POINTER: return "Null pointer error";
    case STRING_ERROR_OUT_OF_MEMORY: return "Out of memory";
    case STRING_ERROR_INVALID_INDEX: return "Invalid index";
    case STRING_ERROR_BUFFER_TOO_SMALL: return "Buffer too small";
    case STRING_ERROR_INVALID_ARGUMENT: return "Invalid argument";
    default: return "Unknown error";
    }
}

/*
 * ==========================================
 * String creation and destruction functions
 * ==========================================
 */

/**
 * @brief Create a new string with specified capacity
 * @param capacity Initial capacity for the string (minimum capacity if 0)
 * @return string_t* Pointer to new string or NULL on failure
 * @details Allocates memory for string structure and data buffer
 */
string_t *string_create_with_capacity(size_t capacity) {
    if (capacity == 0) {
        capacity = STRING_DEFAULT_CAPACITY;
    }

    string_t *str = malloc(sizeof(string_t));
    if (!str) {
        return NULL;
    }

    str->data = malloc(capacity);
    if (!str->data) {
        free(str);
        return NULL;
    }

    str->data[0] = '\0';
    str->length = 0;
    str->capacity = capacity;
    str->is_owner = true;

    return str;
}

/**
 * @brief Create a new empty string with default capacity
 * @return string_t* Pointer to new string or NULL on failure
 * @details Creates string with STRING_DEFAULT_CAPACITY initial capacity
 */
string_t *string_create(void) {
    return string_create_with_capacity(STRING_DEFAULT_CAPACITY);
}

/**
 * @brief Create a new string from a buffer with explicit length
 * @param buffer Source buffer to copy from
 * @param length Number of bytes to copy from buffer
 * @return string_t* Pointer to new string or NULL on failure
 * @details Safely copies specified number of bytes, allowing embedded nulls
 */
string_t *string_create_from_buffer(const char *buffer, size_t length) {
    if (!buffer && length > 0) {
        return NULL;
    }

    size_t capacity = length + 1;
    if (capacity < STRING_DEFAULT_CAPACITY) {
        capacity = STRING_DEFAULT_CAPACITY;
    }

    string_t *str = string_create_with_capacity(capacity);
    if (!str) {
        return NULL;
    }

    if (length > 0) {
        memcpy(str->data, buffer, length);
    }
    str->data[length] = '\0';
    str->length = length;

    return str;
}

/**
 * @brief Create a new string from a null-terminated C string
 * @param cstr Source C string to copy from
 * @return string_t* Pointer to new string or NULL on failure
 * @details Creates string by copying entire C string content
 */
string_t *string_create_from_cstr(const char *cstr) {
    if (!cstr) {
        return string_create();
    }

    size_t len = strlen(cstr);
    return string_create_from_buffer(cstr, len);
}

/**
 * @brief Create a deep copy of an existing string
 * @param src Source string to clone
 * @return string_t* Pointer to new cloned string or NULL on failure
 * @details Creates independent copy with same content as source
 */
string_t *string_clone(const string_t *src) {
    if (!src) {
        return NULL;
    }

    return string_create_from_buffer(src->data, src->length);
}

/**
 * @brief Destroy a string and free its memory
 * @param str String to destroy
 * @details Safely frees both data buffer and string structure
 */
void string_destroy(string_t *str) {
    if (str) {
        if (str->is_owner && str->data) {
            free(str->data);
        }
        free(str);
    }
}

/*
 * =============================
 * String information functions
 * =============================
 */

/**
 * @brief Get the length of a string
 * @param str String to query
 * @return size_t Length of string (0 if string is NULL)
 * @details Returns number of characters excluding null terminator
 */
size_t string_length(const string_t *str) {
    return str ? str->length : 0;
}

/**
 * @brief Get the capacity of a string
 * @param str String to query
 * @return size_t Capacity of string (0 if string is NULL)
 * @details Returns total allocated buffer size
 */
size_t string_capacity(const string_t *str) {
    return str ? str->capacity : 0;
}

/**
 * @brief Check if a string is empty
 * @param str String to check
 * @return bool true if string is empty or NULL, false otherwise
 * @details Returns true for NULL strings or zero-length strings
 */
bool string_is_empty(const string_t *str) {
    return !str || str->length == 0;
}

/**
 * @brief Get C string representation of string
 * @param str String to convert
 * @return const char* Pointer to null-terminated C string
 * @details Returns empty string if input is NULL, never returns NULL
 */
const char *string_cstr(const string_t *str) {
    return str && str->data ? str->data : "";
}

/*
 * ==============================
 * String modification functions
 * ==============================
 */

/**
 * @brief Reserve capacity for string without changing content
 * @param str String to modify
 * @param new_capacity Minimum capacity to reserve
 * @return string_result_t Success or error code
 * @details Ensures string can hold at least new_capacity characters
 */
string_result_t string_reserve(string_t *str, size_t new_capacity) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (new_capacity <= str->capacity) {
        return STRING_SUCCESS;
    }

    return string_ensure_capacity(str, new_capacity);
}

/**
 * @brief Resize string to specified length
 * @param str String to resize
 * @param new_length New length for the string
 * @return string_result_t Success or error code
 * @details Expands with null characters or truncates as needed
 */
string_result_t string_resize(string_t *str, size_t new_length) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    string_result_t result = string_ensure_capacity(str, new_length + 1);
    if (result != STRING_SUCCESS) {
        return result;
    }

    if (new_length > str->length) {
        // Fill new characters with null
        memset(str->data + str->length, '\0', new_length - str->length);
    }

    str->length = new_length;
    str->data[new_length] = '\0';

    return STRING_SUCCESS;
}

/**
 * @brief Clear string content, setting length to zero
 * @param str String to clear
 * @return string_result_t Success or error code
 * @details Keeps allocated memory but resets length to 0
 */
string_result_t string_clear(string_t *str) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    str->length = 0;
    if (str->data) {
        str->data[0] = '\0';
    }

    return STRING_SUCCESS;
}

/**
 * @brief Shrink string capacity to fit current length
 * @param str String to shrink
 * @return string_result_t Success or error code
 * @details Reduces memory usage by reallocating to minimum required size
 */
string_result_t string_shrink_to_fit(string_t *str) {
    if (!str || !str->is_owner) {
        return STRING_ERROR_NULL_POINTER;
    }

    size_t new_capacity = str->length + 1;
    if (new_capacity >= str->capacity) {
        return STRING_SUCCESS;
    }

    char *new_data = realloc(str->data, new_capacity);
    if (!new_data) {
        return STRING_ERROR_OUT_OF_MEMORY;
    }

    str->data = new_data;
    str->capacity = new_capacity;

    return STRING_SUCCESS;
}

/*
 * ============================
 * String assignment functions
 * ============================
 */

/**
 * @brief Assign content from buffer to string
 * @param str Target string to assign to
 * @param buffer Source buffer to copy from
 * @param length Number of bytes to copy
 * @return string_result_t Success or error code
 * @details Replaces entire string content with buffer data
 */
string_result_t string_assign_buffer(string_t *str, const char *buffer, size_t length) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!buffer && length > 0) {
        return STRING_ERROR_INVALID_ARGUMENT;
    }

    string_result_t result = string_ensure_capacity(str, length + 1);
    if (result != STRING_SUCCESS) {
        return result;
    }

    if (length > 0) {
        memcpy(str->data, buffer, length);
    }
    str->data[length] = '\0';
    str->length = length;

    return STRING_SUCCESS;
}

/**
 * @brief Assign C string content to string
 * @param str Target string to assign to
 * @param cstr Source C string to copy from
 * @return string_result_t Success or error code
 * @details Replaces entire string content with C string data
 */
string_result_t string_assign_cstr(string_t *str, const char *cstr) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!cstr) {
        return string_clear(str);
    }

    size_t len = strlen(cstr);
    return string_assign_buffer(str, cstr, len);
}

/**
 * @brief Assign another string's content to string
 * @param dest Target string to assign to
 * @param src Source string to copy from
 * @return string_result_t Success or error code
 * @details Replaces entire destination content with source content
 */
string_result_t string_assign_string(string_t *dest, const string_t *src) {
    if (!dest) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!src) {
        return string_clear(dest);
    }

    return string_assign_buffer(dest, src->data, src->length);
}

/*
 * ===============================
 * String concatenation functions
 * ===============================
 */

/**
 * @brief Append buffer content to end of string
 * @param str Target string to append to
 * @param buffer Source buffer to append from
 * @param length Number of bytes to append
 * @return string_result_t Success or error code
 * @details Adds buffer content to end without replacing existing content
 */
string_result_t string_append_buffer(string_t *str, const char *buffer, size_t length) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!buffer && length > 0) {
        return STRING_ERROR_INVALID_ARGUMENT;
    }

    if (length == 0) {
        return STRING_SUCCESS;
    }

    size_t new_length = str->length + length;
    string_result_t result = string_ensure_capacity(str, new_length + 1);
    if (result != STRING_SUCCESS) {
        return result;
    }

    memcpy(str->data + str->length, buffer, length);
    str->length = new_length;
    str->data[new_length] = '\0';

    return STRING_SUCCESS;
}

/**
 * @brief Append C string content to end of string
 * @param str Target string to append to
 * @param cstr Source C string to append from
 * @return string_result_t Success or error code
 * @details Adds C string content to end without replacing existing content
 */
string_result_t string_append_cstr(string_t *str, const char *cstr) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!cstr) {
        return STRING_SUCCESS;
    }

    size_t len = strlen(cstr);
    return string_append_buffer(str, cstr, len);
}

/**
 * @brief Append another string's content to end of string
 * @param dest Target string to append to
 * @param src Source string to append from
 * @return string_result_t Success or error code
 * @details Adds source string content to end without replacing existing content
 */
string_result_t string_append_string(string_t *dest, const string_t *src) {
    if (!dest) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!src) {
        return STRING_SUCCESS;
    }

    return string_append_buffer(dest, src->data, src->length);
}

/**
 * @brief Append a single character to end of string
 * @param str Target string to append to
 * @param c Character to append
 * @return string_result_t Success or error code
 * @details Adds single character to end of string
 */
string_result_t string_append_char(string_t *str, char c) {
    return string_append_buffer(str, &c, 1);
}

/*
 * ===========================
 * String insertion functions
 * ===========================
 */

/**
 * @brief Insert buffer content at specified position in string
 * @param str Target string to insert into
 * @param index Position to insert at (0-based)
 * @param buffer Source buffer to insert from
 * @param length Number of bytes to insert
 * @return string_result_t Success or error code
 * @details Inserts content at position, shifting existing content right
 */
string_result_t string_insert_buffer(string_t *str, size_t index, const char *buffer, size_t length) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (index > str->length) {
        return STRING_ERROR_INVALID_INDEX;
    }

    if (!buffer && length > 0) {
        return STRING_ERROR_INVALID_ARGUMENT;
    }

    if (length == 0) {
        return STRING_SUCCESS;
    }

    size_t new_length = str->length + length;
    string_result_t result = string_ensure_capacity(str, new_length + 1);
    if (result != STRING_SUCCESS) {
        return result;
    }

    // Move existing characters after insertion point
    if (index < str->length) {
        memmove(str->data + index + length, str->data + index, str->length - index);
    }

    // Insert new characters
    memcpy(str->data + index, buffer, length);
    str->length = new_length;
    str->data[new_length] = '\0';

    return STRING_SUCCESS;
}

/**
 * @brief Insert C string content at specified position in string
 * @param str Target string to insert into
 * @param index Position to insert at (0-based)
 * @param cstr Source C string to insert from
 * @return string_result_t Success or error code
 * @details Inserts C string at position, shifting existing content right
 */
string_result_t string_insert_cstr(string_t *str, size_t index, const char *cstr) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!cstr) {
        return STRING_SUCCESS;
    }

    size_t len = strlen(cstr);
    return string_insert_buffer(str, index, cstr, len);
}

/**
 * @brief Insert another string's content at specified position
 * @param dest Target string to insert into
 * @param index Position to insert at (0-based)
 * @param src Source string to insert from
 * @return string_result_t Success or error code
 * @details Inserts source string at position, shifting existing content right
 */
string_result_t string_insert_string(string_t *dest, size_t index, const string_t *src) {
    if (!dest) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (!src) {
        return STRING_SUCCESS;
    }

    return string_insert_buffer(dest, index, src->data, src->length);
}

/**
 * @brief Insert a single character at specified position in string
 * @param str Target string to insert into
 * @param index Position to insert at (0-based)
 * @param c Character to insert
 * @return string_result_t Success or error code
 * @details Inserts character at position, shifting existing content right
 */
string_result_t string_insert_char(string_t *str, size_t index, char c) {
    return string_insert_buffer(str, index, &c, 1);
}

/*
 * =========================
 * String removal functions
 * =========================
 */

/**
 * @brief Erase characters from string starting at specified position
 * @param str Target string to erase from
 * @param index Starting position for erasure (0-based)
 * @param count Number of characters to erase
 * @return string_result_t Success or error code
 * @details Removes characters and shifts remaining content left
 */
string_result_t string_erase(string_t *str, size_t index, size_t count) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (index >= str->length) {
        return STRING_ERROR_INVALID_INDEX;
    }

    if (count == 0) {
        return STRING_SUCCESS;
    }

    // Adjust count if it goes beyond string end
    if (index + count > str->length) {
        count = str->length - index;
    }

    // Move characters after erased region
    size_t chars_after = str->length - index - count;
    if (chars_after > 0) {
        memmove(str->data + index, str->data + index + count, chars_after);
    }

    str->length -= count;
    str->data[str->length] = '\0';

    return STRING_SUCCESS;
}

/**
 * @brief Remove the last character from string
 * @param str Target string to modify
 * @return string_result_t Success or error code
 * @details Decreases string length by one, removing last character
 */
string_result_t string_pop_back(string_t *str) {
    if (!str || str->length == 0) {
        return STRING_ERROR_INVALID_INDEX;
    }

    str->length--;
    str->data[str->length] = '\0';

    return STRING_SUCCESS;
}

/*
 * ========================
 * String access functions
 * ========================
 */

/**
 * @brief Get character at specified position in string
 * @param str Source string to access
 * @param index Position of character to retrieve (0-based)
 * @return char Character at position, or '\0' if index is invalid
 * @details Safe character access with bounds checking
 */
char string_at(const string_t *str, size_t index) {
    if (!str || index >= str->length) {
        return '\0';
    }

    return str->data[index];
}

/**
 * @brief Set character at specified position in string
 * @param str Target string to modify
 * @param index Position of character to set (0-based)
 * @param c New character value
 * @return string_result_t Success or error code
 * @details Safe character modification with bounds checking
 */
string_result_t string_set_at(string_t *str, size_t index, char c) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (index >= str->length) {
        return STRING_ERROR_INVALID_INDEX;
    }

    str->data[index] = c;
    return STRING_SUCCESS;
}

/*
 * ============================
 * String comparison functions
 * ============================
 */

/**
 * @brief Compare two strings lexicographically
 * @param str1 First string to compare
 * @param str2 Second string to compare
 * @return int Negative if str1 < str2, 0 if equal, positive if str1 > str2
 * @details Performs safe comparison handling NULL pointers
 */
int string_compare(const string_t *str1, const string_t *str2) {
    if (!str1 && !str2) return 0;
    if (!str1) return -1;
    if (!str2) return 1;

    size_t min_len = str1->length < str2->length ? str1->length : str2->length;
    int result = memcmp(str1->data, str2->data, min_len);

    if (result == 0) {
        if (str1->length < str2->length) return -1;
        if (str1->length > str2->length) return 1;
    }

    return result;
}

/**
 * @brief Compare string with C string lexicographically
 * @param str String to compare
 * @param cstr C string to compare with
 * @return int Negative if str < cstr, 0 if equal, positive if str > cstr
 * @details Performs safe comparison handling NULL pointers
 */
int string_compare_cstr(const string_t *str, const char *cstr) {
    if (!str && !cstr) return 0;
    if (!str) return -1;
    if (!cstr) return 1;

    return strcmp(str->data, cstr);
}

/**
 * @brief Check if two strings are equal
 * @param str1 First string to compare
 * @param str2 Second string to compare
 * @return bool true if strings are equal, false otherwise
 * @details Convenient equality check using string comparison
 */
bool string_equals(const string_t *str1, const string_t *str2) {
    return string_compare(str1, str2) == 0;
}

/**
 * @brief Check if string equals C string
 * @param str String to compare
 * @param cstr C string to compare with
 * @return bool true if strings are equal, false otherwise
 * @details Convenient equality check using C string comparison
 */
bool string_equals_cstr(const string_t *str, const char *cstr) {
    return string_compare_cstr(str, cstr) == 0;
}

/*
 * ===========================
 * String searching functions
 * ===========================
 */

/**
 * @brief Find first occurrence of character in string
 * @param str String to search in
 * @param c Character to search for
 * @param start_pos Starting position for search (0-based)
 * @return size_t Position of character or STRING_NPOS if not found
 * @details Searches forward from start_pos for character
 */
size_t string_find_char(const string_t *str, char c, size_t start_pos) {
    if (!str || start_pos >= str->length) {
        return STRING_NPOS;
    }

    for (size_t i = start_pos; i < str->length; i++) {
        if (str->data[i] == c) {
            return i;
        }
    }

    return STRING_NPOS;
}

/**
 * @brief Find first occurrence of C string substring in string
 * @param str String to search in
 * @param substr Substring to search for
 * @param start_pos Starting position for search (0-based)
 * @return size_t Position of substring or STRING_NPOS if not found
 * @details Searches forward from start_pos for substring
 */
size_t string_find_cstr(const string_t *str, const char *substr, size_t start_pos) {
    if (!str || !substr || start_pos >= str->length) {
        return STRING_NPOS;
    }

    size_t substr_len = strlen(substr);
    if (substr_len == 0) {
        return start_pos;
    }

    if (start_pos + substr_len > str->length) {
        return STRING_NPOS;
    }

    for (size_t i = start_pos; i <= str->length - substr_len; i++) {
        if (memcmp(str->data + i, substr, substr_len) == 0) {
            return i;
        }
    }

    return STRING_NPOS;
}

/**
 * @brief Find first occurrence of string substring in string
 * @param str String to search in
 * @param substr String substring to search for
 * @param start_pos Starting position for search (0-based)
 * @return size_t Position of substring or STRING_NPOS if not found
 * @details Searches forward from start_pos for substring
 */
size_t string_find_string(const string_t *str, const string_t *substr, size_t start_pos) {
    if (!str || !substr) {
        return STRING_NPOS;
    }

    return string_find_cstr(str, substr->data, start_pos);
}

/**
 * @brief Find last occurrence of character in string (reverse search)
 * @param str String to search in
 * @param c Character to search for
 * @param start_pos Starting position for reverse search (STRING_NPOS for end)
 * @return size_t Position of character or STRING_NPOS if not found
 * @details Searches backward from start_pos for character
 */
size_t string_rfind_char(const string_t *str, char c, size_t start_pos) {
    if (!str || str->length == 0) {
        return STRING_NPOS;
    }

    if (start_pos >= str->length) {
        start_pos = str->length - 1;
    }

    for (size_t i = start_pos + 1; i > 0; i--) {
        if (str->data[i - 1] == c) {
            return i - 1;
        }
    }

    return STRING_NPOS;
}

/*
 * =========================
 * String utility functions
 * =========================
 */

/**
 * @brief Convert all characters in string to uppercase
 * @param str String to convert
 * @return string_result_t Success or error code
 * @details Modifies string in-place using toupper() function
 */
string_result_t string_to_upper(string_t *str) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    for (size_t i = 0; i < str->length; i++) {
        str->data[i] = toupper(str->data[i]);
    }

    return STRING_SUCCESS;
}

/**
 * @brief Convert all characters in string to lowercase
 * @param str String to convert
 * @return string_result_t Success or error code
 * @details Modifies string in-place using tolower() function
 */
string_result_t string_to_lower(string_t *str) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    for (size_t i = 0; i < str->length; i++) {
        str->data[i] = tolower(str->data[i]);
    }

    return STRING_SUCCESS;
}

/**
 * @brief Remove leading and trailing whitespace from string
 * @param str String to trim
 * @return string_result_t Success or error code
 * @details Removes spaces, tabs, newlines, and other whitespace characters
 */
string_result_t string_trim(string_t *str) {
    if (!str || str->length == 0) {
        return STRING_ERROR_NULL_POINTER;
    }

    // Find first non-whitespace character
    size_t start = 0;
    while (start < str->length && isspace(str->data[start])) {
        start++;
    }

    // Find last non-whitespace character
    size_t end = str->length;
    while (end > start && isspace(str->data[end - 1])) {
        end--;
    }

    // Move trimmed content to beginning if needed
    if (start > 0) {
        memmove(str->data, str->data + start, end - start);
    }

    str->length = end - start;
    str->data[str->length] = '\0';

    return STRING_SUCCESS;
}

/**
 * @brief Replace all occurrences of one character with another
 * @param str String to modify
 * @param old_char Character to replace
 * @param new_char Character to replace with
 * @return string_result_t Success or error code
 * @details Replaces all instances of old_char with new_char in string
 */
string_result_t string_replace_char(string_t *str, char old_char, char new_char) {
    if (!str) {
        return STRING_ERROR_NULL_POINTER;
    }

    for (size_t i = 0; i < str->length; i++) {
        if (str->data[i] == old_char) {
            str->data[i] = new_char;
        }
    }

    return STRING_SUCCESS;
}

/*
 * =========================
 * String copying functions
 * =========================
 */

/**
 * @brief Safely copy string content to C-style buffer
 * @param str Source string to copy from
 * @param buffer Destination buffer to copy to
 * @param buffer_size Size of destination buffer
 * @return string_result_t Success or error code indicating truncation
 * @details Copies as much as possible, always null-terminates buffer
 */
string_result_t string_copy_to_buffer(const string_t *str, char *buffer, size_t buffer_size) {
    if (!str || !buffer) {
        return STRING_ERROR_NULL_POINTER;
    }

    if (buffer_size == 0) {
        return STRING_ERROR_BUFFER_TOO_SMALL;
    }

    size_t copy_length = str->length;
    if (copy_length >= buffer_size) {
        copy_length = buffer_size - 1;
    }

    memcpy(buffer, str->data, copy_length);
    buffer[copy_length] = '\0';

    return copy_length == str->length ? STRING_SUCCESS : STRING_ERROR_BUFFER_TOO_SMALL;
}

/*
 * ============================
 * String formatting functions
 * ============================
 */

/**
 * @brief Format string using printf-style format specifiers
 * @param str Target string to store formatted result
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 * @return string_result_t Success or error code
 * @details Replaces entire string content with formatted result
 */
string_result_t string_format(string_t *str, const char *format, ...) {
    if (!str || !format) {
        return STRING_ERROR_NULL_POINTER;
    }

    va_list args;
    va_start(args, format);

    // Calculate required length
    va_list args_copy;
    va_copy(args_copy, args);
    int required_length = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (required_length < 0) {
        va_end(args);
        return STRING_ERROR_INVALID_ARGUMENT;
    }

    string_result_t result = string_ensure_capacity(str, required_length + 1);
    if (result != STRING_SUCCESS) {
        va_end(args);
        return result;
    }

    vsnprintf(str->data, required_length + 1, format, args);
    str->length = required_length;

    va_end(args);
    return STRING_SUCCESS;
}

/**
 * @brief Append formatted content to end of string
 * @param str Target string to append to
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 * @return string_result_t Success or error code
 * @details Appends formatted result to existing string content
 */
string_result_t string_append_format(string_t *str, const char *format, ...) {
    if (!str || !format) {
        return STRING_ERROR_NULL_POINTER;
    }

    va_list args;
    va_start(args, format);

    // Calculate required length
    va_list args_copy;
    va_copy(args_copy, args);
    int required_length = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (required_length < 0) {
        va_end(args);
        return STRING_ERROR_INVALID_ARGUMENT;
    }

    size_t new_length = str->length + required_length;
    string_result_t result = string_ensure_capacity(str, new_length + 1);
    if (result != STRING_SUCCESS) {
        va_end(args);
        return result;
    }

    vsnprintf(str->data + str->length, required_length + 1, format, args);
    str->length = new_length;

    va_end(args);
    return STRING_SUCCESS;
}
