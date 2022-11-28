//
// Created by NHTHS on 25.11.22.
//

#include "Exception.h"

Exception::Exception(std::string &&msg) noexcept : msg(std::move(msg)){}

std::string Exception::what() {
    return msg;
}

Exception::Exception(const std::string &whatStr) noexcept : msg(whatStr) { }


ArgCountException::ArgCountException(std::string &&msg) noexcept: Exception(msg) {
}

ArgCountException::ArgCountException(const std::string &whatStr) noexcept: Exception(whatStr) {
}


WrongKeyException::WrongKeyException(std::string &&msg) noexcept: Exception(msg) {
}

WrongKeyException::WrongKeyException(const std::string &whatStr) noexcept: Exception(whatStr) {
}

WrongArchiverMode::WrongArchiverMode(std::string &&msg) noexcept: Exception(msg) {

}

WrongArchiverMode::WrongArchiverMode(const std::string &whatStr) noexcept: Exception(whatStr) {

}

FileOpenError::FileOpenError(std::string &&msg) noexcept: FileException(msg) {

}

FileOpenError::FileOpenError(const std::string &whatStr) noexcept: FileException(whatStr) {

}

FileException::FileException(const std::string &whatStr) noexcept: Exception(whatStr) {

}

FileException::FileException(std::string &&msg) noexcept: Exception(msg) {

}
