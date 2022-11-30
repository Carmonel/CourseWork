#ifndef COURSEWORK_CONFIGMAPPER_H
#define COURSEWORK_CONFIGMAPPER_H

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

class ConfigMapper {
public:
    static Config readConfig(int argc, char *argv[]);
};


#endif //COURSEWORK_CONFIGMAPPER_H
