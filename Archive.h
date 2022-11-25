//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_ARCHIVE_H
#define COURSEWORK_ARCHIVE_H

#include <string>
#include <iostream>
#include <fstream>

class Archive {
private:
    std::ifstream &input;
public:
    explicit Archive(std::ifstream &input);
    ~Archive() = default;

    void compress(std::ofstream &output);
    void decompress(std::ofstream &output);
};

#endif //COURSEWORK_ARCHIVE_H
