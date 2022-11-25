#include "ArciverExecutor.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "Fano.h"

using std::string;

#include "Exception.h"
#include "Archive.h"

void ArchiverExecutor::execute(Config &config) {
    if (config.mode == Config::COMPRESS) {

    } else if (config.mode == Config::DECOMPRESS) {

    } else {
        throw WrongArchiverMode("No execute mode!!!");
    }
}
