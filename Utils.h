//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_UTILS_H
#define COURSEWORK_UTILS_H

#include <string>

class Utils {
public:
    static std::string createDecompressedFileName(std::string path);
    static std::string createCompressedFileName(std::string path);
};


#endif //COURSEWORK_UTILS_H