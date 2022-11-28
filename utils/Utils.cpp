//
// Created by NHTHS on 25.11.22.
//

#include "Utils.h"

using namespace std;

// Переопределение пути в вид \\root\\DECOMPRESS[file]
std::string Utils::createDecompressedFileName(std::string path) {
    path.erase(path.length()-8);
    int it = path.length();
    while (path[it] != '/') it--;
    it++;
    path.insert(it, "DECOMPRESS");

    return path;
}

std::string Utils::createCompressedFileName(std::string path) {
    return path + ".archive";
}
