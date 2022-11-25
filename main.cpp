#include "ArciverExecutor.h"
#include <iostream>
#include <string>

#include "Exception.h"
#include "SyntaxChecker.h"

#include "ConfigMapper.h"

#include "Log.h"

using namespace std;

int main(int argc, char *argv[]) {
    try {
        Config config = ConfigMapper::readConfig(argc, argv);
        Log::setLogLevel(config.verbose ? LogLevel::Verbose : LogLevel::Error);
        ArchiverExecutor::execute(config);
    } catch (Exception &e) {
        cerr << e.what() << endl;
        cerr << SyntaxChecker::printErr() << endl;
        return -1;
    }

    return 0;
}