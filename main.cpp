#include "ArchiverExecutor.h"
#include <iostream>
#include <string>

#include "utils/Exception.h"

#include "utils/ConfigMapper.h"

#include "utils/Log.h"

using namespace std;

int main(int argc, char *argv[]) {
    try {
        Config config = ConfigMapper::readConfig(argc, argv);
        Log::setLogLevel(config.logLevel);
        ArchiverExecutor::execute(config);
    } catch (Exception &e) {
        cerr << e.what() << endl;
        cerr << "Wrong arguments\n"
                "Use: algorithmFano.exe [mode] [view] [path]\n"
                "Example 1: algorithmFano.exe -c -v c:/file.txt\n"
                "Example 2: algorithmFano.exe -d c:/file.txt.archive" << endl;
        return -1;
    }

    return 0;
}