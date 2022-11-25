//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_LOG_H
#define COURSEWORK_LOG_H

#include <string>

enum LogLevel {
    Error = 0, Warning = 1, Info = 2, Verbose = 3, Debug = 4
};

constexpr static const LogLevel priority[] = {Error, Warning, Info, Verbose, Debug};

class Log {
private:
    static LogLevel logLevel;
public:
    static void setLogLevel(const LogLevel &logLevel);

    static void e(const std::string msg);
    static void e(const char* msg);

    static void w(const std::string msg);
    static void w(const char* msg);

    static void i(const std::string msg);
    static void i(const char* msg);

    static void v(const std::string msg);
    static void v(const char* msg);

    static void d(const std::string msg);
    static void d(const char* msg);
};

#endif //COURSEWORK_LOG_H