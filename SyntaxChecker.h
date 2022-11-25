#ifndef COURSEWORK_SYNTAXCHECKER_H
#define COURSEWORK_SYNTAXCHECKER_H

#include <string>
#include "Exception.h"

class SyntaxChecker {
public:
    static std::string printErr();

    static void checkArgsCount(int argc);
};


#endif //COURSEWORK_SYNTAXCHECKER_H
