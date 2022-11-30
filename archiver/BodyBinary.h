//
// Created by NHTHS on 30.11.22.
//

#ifndef COURSEWORK_BODYBINARY_H
#define COURSEWORK_BODYBINARY_H

#include <vector>
#include <fstream>
#include "Tree.h"

class BodyBinary {
public:
    static void writeCompressedToFile(std::ifstream &source, std::ostream &dest,
                               std::vector<std::pair<unsigned char, std::vector<bool>>> &codes);
    static void readFromCompressedFile(std::ifstream &source, std::ostream &dest,
                                Node *fanoTree);
};


#endif //COURSEWORK_BODYBINARY_H
