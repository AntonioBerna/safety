# safety

![GitHub repo size](https://img.shields.io/github/repo-size/AntonioBerna/safety)
![GitHub License](https://img.shields.io/github/license/AntonioBerna/safety)
![GitHub Created At](https://img.shields.io/github/created-at/antonioberna/safety)

## 🎯 Project Vision

**Safety** is a comprehensive C library designed to make C programming more secure and reliable by providing memory-safe alternatives to standard C operations.
The library focuses on preventing common vulnerabilities such as buffer overflows, memory leaks, null pointer dereferences, and other memory-related security issues.

C is a powerful and efficient programming language, but its manual memory management and lack of built-in safety mechanisms make it prone to security vulnerabilities and bugs.
The **safety** library aims to bridge this gap by providing:

- **Memory-safe data structures** with automatic memory management;
- **Bounds-checked operations** to prevent buffer overflows;
- **Comprehensive error handling** with detailed error reporting;
- **Modern C best practices** while maintaining performance;
- **Drop-in replacements** for common C patterns.

## ✨ Features

The **safety** library provides a comprehensive suite of memory-safe implementations that address the most critical vulnerabilities in C programming.
Each module implements proven safety strategies with rigorous bounds checking, automatic memory management, and comprehensive error handling.

### Currently Implemented

#### 🧵 Safe Strings (`safe_strings/`)
**Complete implementation** of memory-safe string operations with:
- **Automatic memory management** - No manual allocation/deallocation required
- **Dynamic resizing** - Strings grow automatically as needed with efficient growth strategies
- **Bounds checking** - All operations are bounds-checked to prevent buffer overflows
- **Rich API** - Comprehensive set of string operations (create, modify, search, format)
- **Error handling** - Detailed error codes with human-readable messages
- **Performance optimized** - Efficient memory usage patterns and minimal reallocations

### Planned Safety Implementations

#### 🔐 Safe User Input
- **Input validation** and sanitization to prevent injection attacks
- **Buffer overflow protection** for user input handling
- **Safe parsing** of numeric and structured data
- **Command line argument validation** with type checking

#### 📊 Safe Dynamic Arrays
- **Bounds-checked access** to prevent array overflow/underflow
- **Automatic memory management** with safe resizing
- **Type-safe operations** with compile-time and runtime checks
- **Iterator safety** to prevent invalid access patterns

#### 🔒 Safe System V Semaphores
- **Deadlock detection** and prevention mechanisms
- **Resource leak protection** with automatic cleanup
- **Timeout handling** to prevent indefinite blocking
- **Safe inter-process synchronization** with error recovery

#### 📁 Safe File I/O
- **Path traversal protection** to prevent directory escape attacks
- **Safe file operations** with automatic buffer management
- **Access permission validation** before file operations
- **Atomic file operations** to prevent data corruption

#### 🌐 Safe Network Programming
- **Buffer overflow protection** for socket operations
- **Safe protocol parsing** with input validation
- **Connection management** with timeout and error handling
- **Secure data transmission** patterns

#### 💾 Safe Memory Management
- **Use-after-free detection** with pointer validation
- **Double-free prevention** mechanisms
- **Memory leak tracking** and automatic cleanup
- **Safe pointer arithmetic** with bounds checking

## 🚀 Getting Started

Here's how to get started:

```bash
# Clone the repository
git clone https://github.com/yourusername/safety.git
cd safety/
```

### 🧵 Safe Strings Usage

#### :memo: Full example

You can build and run the example program that demonstrates the usage of safe strings:

```bash
cd safe_strings/examples/
cmake -B build -S .
cmake --build build

# Run the example
./build/full_example

# Valgrind check
valgrind --leak-check=full --track-origins=yes ./build/full_example
```

#### :test_tube: Testing

You can test the safe strings implementation by running the following commands:

```bash
cd safe_strings/tests/
cmake -B build -S .
cmake --build build

# Run tests
./build/safe_strings_test

# Valgrind check
valgrind --leak-check=full --track-origins=yes ./build/safe_strings_test
```
