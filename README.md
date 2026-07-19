# C++ Encryption/Decryption Project

A C++ console application that encrypts or decrypts files in a directory using a simple byte-based Caesar-style cipher. The project also demonstrates basic file handling, environment-based configuration, and multithreaded task execution.

## Features

- Encrypts or decrypts files recursively within a chosen directory
- Uses a simple custom encryption algorithm based on an integer key from the `.env` file
- Supports both:
  - batch processing through the interactive executable
  - single-file processing through a command-line utility
- Uses `std::thread` for concurrent execution of submitted tasks

## Project Structure

```text
.
├── main.cpp
├── Makefile
├── .env
├── README.md
├── src/
│   └── app/
│       ├── encryptDecrypt/
│       │   ├── Cryption.cpp
│       │   ├── Cryption.hpp
│       │   └── CryptionMain.cpp
│       ├── file_handling/
│       │   ├── IO.cpp
│       │   ├── IO.hpp
│       │   └── ReadEnv.cpp
│       └── processes/
│           ├── ProcessManagement.cpp
│           ├── ProcessManagement.hpp
│           └── Task.hpp
└── test/
    └── sample input files
```

## How It Works

### 1. Input and Task Creation
The application scans a target directory for regular files and creates a `Task` object for each one. Each task contains:
- the file path
- the requested action (`ENCRYPT` or `DECRYPT`)
- an open file stream

### 2. Processing
Each task is submitted to `ProcessManagement`, which starts a separate `std::thread` for execution. The worker thread calls the encryption/decryption logic.

### 3. Encryption Logic
The encryption logic reads the file as binary data, applies the key to each byte, and writes the transformed content back to disk. A marker byte is used to detect whether a file is already encrypted, helping avoid double encryption.

### 4. Key Configuration
The encryption key is read from the `.env` file at the project root.

## Build Instructions

Use the provided Makefile:

```bash
mingw32-make
```

This builds two executables:
- `encrypt_decrypt.exe` for batch directory processing
- `cryption.exe` for single-file command-line processing

## Run the Application

### Batch Mode
Run the main executable:

```bash
./encrypt_decrypt
```

You will be prompted for:
1. the directory path to scan
2. the action: `encrypt` or `decrypt`

### Single-File Mode
Run the CLI utility:

```bash
./cryption "path/to/file,ENCRYPT"
```

## Example

```bash
./encrypt_decrypt
Enter the directory path: test
Enter the action (encryption/decryption) : encrypt
```

## Notes

- The current implementation uses detached threads for each task, which is suitable for simple concurrent processing.
- The project is designed as a learning-style C++ example and can be extended with a thread pool or a more robust task queue.

## Dependencies

- C++17 compiler
- MinGW `g++` (for Windows builds)
