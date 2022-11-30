#include "ConfigMapper.h"
#include "Exception.h"

Config ConfigMapper::readConfig(int argc, char **argv) {
    if ((argc != 3) && (argc != 4)){
        throw ArgCountException("Arguments count must be 2 or 3");
    }

    Config config;

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);

        if (arg == "-c"){
            config.mode = Config::COMPRESS;
        } else if (arg == "-d"){
            config.mode = Config::DECOMPRESS;
        } else if (arg == "-v") {
            config.logLevel = Verbose;
        } else if (arg == "-dbg"){
            config.logLevel = Debug;
        } else {
            if (config.path.empty()) {
                config.path = arg;
            } else {
                throw WrongKeyException("Use '-d' or '-c'!\nMaybe not english letter?");
            }
        }
    }

    return config;
}
