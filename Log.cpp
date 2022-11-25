#include "Log.h"

#include "iostream"

LogLevel Log::logLevel = Error;

using namespace std;

void Log::setLogLevel(const LogLevel &logLevel) {
    Log::logLevel = logLevel;
}

void Log::e(const std::string msg) {
    if (Error <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::e(const char *msg) {
    if (Error <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::w(const std::string msg) {
    if (Warning <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::w(const char *msg) {
    if (Warning <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::i(const std::string msg) {
    if (Info <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::i(const char *msg) {
    if (Info <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::v(const std::string msg) {
    if (Verbose <= Log::logLevel) {
        cerr << msg << endl;
    }
}

void Log::v(const char *msg) {
    if (Verbose <= Log::logLevel) {
        cerr << msg << endl;
    }
}


