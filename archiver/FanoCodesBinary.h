//
// Created by NHTHS on 30.11.22.
//

#ifndef COURSEWORK_FANOCODESBINARY_H
#define COURSEWORK_FANOCODESBINARY_H

#include <vector>
#include <fstream>
#include "Tree.h"

class FanoCodesBinary {
public:
    static std::vector<std::pair<unsigned char, std::vector<bool>>> makeFromFile(std::ifstream &is);

    static Node readFromFile(std::ifstream &source);
    static void writeToFile(std::ofstream &os, std::vector<std::pair<unsigned char, std::vector<bool>>> &codes);
};


#endif //COURSEWORK_FANOCODESBINARY_H
