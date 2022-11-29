#ifndef KURSOVAYA_FANO_H
#define KURSOVAYA_FANO_H

#include "Tree.h"
#include "utils/Log.h"
#include <vector>

#include <iostream>

class Fano {
private:
    std::vector< std::pair<unsigned char, std::vector<bool>> > storedCode;
public:
    Fano(std::istream &input);

    friend std::ostream& operator<<(std::ostream& outLine, const Fano& outClass);
    void writeKeys(std::ostream &out);
    void generateArchived(std::istream &input, std::ostream &out);

    ~Fano();
};

inline std::ostream& operator<<(std::ostream& outLine, const Fano& outClass){
    for (auto &i : outClass.storedCode){
        outLine << (int)i.first << ".'";
        for (bool b : i.second) {
            outLine << b;
        }
        outLine << "'" << std::endl;
    }
    return outLine;
}

#endif //KURSOVAYA_FANO_H