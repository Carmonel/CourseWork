//
// Created by NHTHS on 25.11.22.
//

#include "SyntaxChecker.h"
#include "Exception.h"

using namespace std;

static const string errMsg("Wrong arguments\n"
                     "Use: algorithmFano.exe [mode] [view] [path]\n"
                     "Example 1: algorithmFano.exe -c -v c:/file.txt\n"
                     "Example 2: algorithmFano.exe -d c:/file.txt.archive");

string SyntaxChecker::printErr() {
    return errMsg;
}

void SyntaxChecker::checkArgsCount(int argc) {
    if ((argc != 3) && (argc != 4)){
        throw ArgCountException("Arguments count must be 2 or 3");
    }
}
