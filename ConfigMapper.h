//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_CONFIGMAPPER_H
#define COURSEWORK_CONFIGMAPPER_H

#include "Config.h"

class ConfigMapper {
public:
    static Config readConfig(int argc, char *argv[]);
};


#endif //COURSEWORK_CONFIGMAPPER_H
