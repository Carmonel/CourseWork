//
// Created by NHTHS on 25.11.22.
//

#ifndef COURSEWORK_ARCHIVE_H
#define COURSEWORK_ARCHIVE_H

#include <string>

class Archive {
private:
    std::string path;
public:
    explicit Archive(const std::string &path);
    ~Archive() = default;

    void compress(const std::string &outPath);
    void decompress(const std::string &outPath);
};

#endif //COURSEWORK_ARCHIVE_H
