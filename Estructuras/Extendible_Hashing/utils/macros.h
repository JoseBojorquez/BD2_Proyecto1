#ifndef BD2_PROYECTO1_MACROS_H
#define BD2_PROYECTO1_MACROS_H

#include <iostream>
#include <fstream>

// File I/O definitions

#define OPEN_FILE(file, filename, flags)                        \
    file.open(filename, flags);                                 \
    if (!file.is_open()) {                                      \
        throw std::runtime_error("open file ERROR-MACRO-ONE");  \
    }

#define CREATE_FILE_IF_NOT_EXISTS(file, filename)               \
    file.open(filename, std::ios::app);                         \
    if (!file.is_open()) {                                      \
        throw std::runtime_error("open file ERROR-MACRO_TWO");  \
    }                                                           \
    file.close();

#define SEEK_ALL(file, pos) \
    file.seekg(pos);       \
    file.seekp(pos);

#define SEEK_ALL_RELATIVE(file, pos, relative) \
    file.seekg(pos, relative);                 \
    file.seekp(pos, relative);

// DEBUGGING

#define PRINT_FLAGS(file) \
    std::cout << std::boolalpha << "Good: " << file.good() << " Eof: " << file.eof() << " Bad: " << file.bad() << " Fail: " << file.fail() << std::endl;

#define PRINT_SIZE(T) \
    std::cout << "Size: " << sizeof(T) << std::endl;

#define PRINT_TELL(file) \
    std::cout << "tellg: " << file.tellg() << " tellp: " << file.tellp() << std::endl;

#endif //BD2_PROYECTO1_MACROS_H
