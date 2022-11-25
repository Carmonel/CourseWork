//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_CONFIG_H
#define COURSEWORK_CONFIG_H

#include <string>

#include "Log.h"

struct Config {
    enum Mode{
        COMPRESS,DECOMPRESS,UNSPECIFIED
    };

    Mode mode;
    LogLevel logLevel;
    std::string path;

    inline Config(){
        mode = UNSPECIFIED;
        logLevel = Error;
        path = "";
    }
    inline Config(Mode &mode, LogLevel logLevel, std::string &path)
        : mode(mode), logLevel(logLevel), path(path){}
};

#endif //COURSEWORK_CONFIG_H
