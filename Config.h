//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_CONFIG_H
#define COURSEWORK_CONFIG_H

#include <string>

struct Config {
    enum Mode{
        COMPRESS,DECOMPRESS,UNSPECIFIED
    };

    Mode mode;
    bool verbose;
    std::string path;

    inline Config(){
        mode = UNSPECIFIED;
        verbose = false;
        path = "";
    }
    inline Config(Mode &mode, bool verbose, std::string &path)
        : mode(mode), verbose(verbose), path(path){}
};

#endif //COURSEWORK_CONFIG_H
