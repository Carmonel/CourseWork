#ifndef KURSOVAYA_FANO_H
#define KURSOVAYA_FANO_H

#include "tree.h"
#include <vector>

class Fano {
private:
    vector< pair<int, int> > storedUsages;
    vector< pair<char, string> > storedCode;
public:
    Fano(const string& pathToFile);

    friend std::ostream& operator<<(std::ostream& outLine, const Fano& outClass);
    void generateKeyFile(const string& path);
    void generateInefficient(const string& path);
    void generateArchived(const string &pathToFile);
    bool isEfficiency();

    ~Fano();
};

string intToHexStr(int value);

inline std::ostream& operator<<(std::ostream& outLine, const Fano& outClass){
    std::cout << "storedUsages:" << std::endl;
    for (auto &i : outClass.storedUsages){
        outLine << "'" << (int)i.first << "'" << "[" << (char)i.first << "]";
        outLine << " = ";
        outLine << i.second;
        outLine << std::endl;
    }
    std::cout << "Code:" << std::endl;
    for (auto &i : outClass.storedCode){
        outLine << (int)i.first << ".'" << i.second << "'" << std::endl;
    }
    return outLine;
}

void startAlgorithm(const string& path);
vector< pair<int, int> >::iterator findVectorIt(vector<pair<int, int>>* vec, int value);

#endif //KURSOVAYA_FANO_H