# safety

![GitHub repo size](https://img.shields.io/github/repo-size/AntonioBerna/safety)
![GitHub License](https://img.shields.io/github/license/AntonioBerna/safety)
![GitHub Created At](https://img.shields.io/github/created-at/antonioberna/safety)

## :brain: Overview

**Safety** is a comprehensive C library designed to make C programming more secure and reliable by providing memory-safe alternatives to standard C operations.
The library focuses on preventing common vulnerabilities such as buffer overflows, memory leaks, null pointer dereferences, and other memory-related security issues.

C is a powerful and efficient programming language, but its manual memory management and lack of built-in safety mechanisms make it prone to security vulnerabilities and bugs.
The **safety** library aims to bridge this gap by providing:

- **Memory-safe data structures** with automatic memory management;
- **Bounds-checked operations** to prevent buffer overflows;
- **Comprehensive error handling** with detailed error reporting;
- **Modern C best practices** while maintaining performance;
- **Drop-in replacements** for common C patterns.

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
cd sstring/examples/
cmake -B build -S .
cmake --build build

# Run the example
./build/full-example

# Valgrind check
valgrind --leak-check=full --track-origins=yes ./build/full-example
```

#### :test_tube: Testing

You can test the safe strings implementation by running the following commands:

```bash
cd sstring/tests/
cmake -B build -S .
cmake --build build

# Run tests
./build/sstring-test

# Valgrind check
valgrind --leak-check=full --track-origins=yes ./build/sstring-test
```
