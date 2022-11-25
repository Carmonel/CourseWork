//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_EXCEPTION_H
#define COURSEWORK_EXCEPTION_H

#include <string>

class Exception : std::exception{
private:
    std::string msg;
public:
    explicit Exception(std::string &&msg) noexcept;
    explicit Exception(const std::string &whatStr) noexcept;
    ~Exception() noexcept override = default;
    std::string what();
};

class ArgCountException : public Exception{
public:
    explicit ArgCountException(std::string &&msg) noexcept;
    explicit ArgCountException(const std::string &whatStr) noexcept;
    ~ArgCountException() noexcept override = default;
};

class WrongKeyException : public Exception {
public:
    explicit WrongKeyException(std::string &&msg) noexcept;
    explicit WrongKeyException(const std::string &whatStr) noexcept;
    ~WrongKeyException() noexcept override = default;
};

class WrongArchiverMode : public Exception {
public:
    explicit WrongArchiverMode(std::string &&msg) noexcept;
    explicit WrongArchiverMode(const std::string &whatStr) noexcept;
    ~WrongArchiverMode() noexcept override = default;
};

class FileException : public Exception {
public:
    explicit FileException(std::string &&msg) noexcept;
    explicit FileException(const std::string &whatStr) noexcept;
    ~FileException() noexcept override = default;
};

class FileOpenError : public FileException {
public:
    explicit FileOpenError(std::string &&msg) noexcept;
    explicit FileOpenError(const std::string &whatStr) noexcept;
    ~FileOpenError() noexcept override = default;
};

#endif //COURSEWORK_EXCEPTION_H
