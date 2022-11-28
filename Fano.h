#ifndef KURSOVAYA_FANO_H
#define KURSOVAYA_FANO_H

#include "Tree.h"
#include <vector>

#include <iostream>

class Fano {
private:
    std::vector< std::pair<unsigned char, std::vector<bool>> > storedCode;
public:
    Fano(std::istream &input);

    friend std::ostream& operator<<(std::ostream& outLine, const Fano& outClass);
    void writeKeys(std::ostream &out);
    void generateInefficient(std::istream &input, std::ostream &out);
    void generateArchived(std::istream &input, std::ostream &out);
    bool isEfficiency();

    ~Fano();
};

inline std::ostream& operator<<(std::ostream& outLine, const Fano& outClass){
    std::cout << "Code:" << std::endl;
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