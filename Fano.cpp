#include <fstream>
#include <iostream>
#include "Fano.h"
#include "Log.h"
#include "Exception.h"
#include <algorithm>

using namespace std;

// https://iq.opengenus.org/sorting-vector-in-cpp/
bool compare(pair<char, int> a, pair<char, int> b){
    return a.second > b.second;
}

Fano::Fano(std::istream &input) {
    vector< pair<int, int> >::iterator iterator;

    // Ввод символов
    int ch;
    while (input.read((char*)&ch, 1)){
        Log::d("char " + to_string(ch));
        // Если такой символ уже есть
        iterator = findVectorIt(&storedUsages, ch);
        if (iterator == storedUsages.end()) storedUsages.emplace_back(ch, 1);
        else iterator->second++;
    }
    sort(storedUsages.begin(), storedUsages.end(), compare); // https://iq.opengenus.org/sorting-vector-in-cpp/

    Node* head = new Node;

    int* temporaryFreqArray = new int[storedUsages.size()];
    for (int i = 0; i < storedUsages.size(); i++){
        temporaryFreqArray[i] = storedUsages.at(i).second;
    }
    makeTree(temporaryFreqArray, storedUsages.size(), head);

    // Сейчас обход по дереву, где char и binary string
    returnValues(head, 0, &storedCode);

    auto itUsages = storedUsages.begin();
    auto itCode = storedCode.begin();
    while (itUsages != storedUsages.end()){
        itCode->first = itUsages->first;
        ++itUsages;
        ++itCode;
    }

    // Исправление пропуска
    itCode = storedCode.begin();
    while (itCode != storedCode.end()){
        if (itCode->second.empty()) break;
        ++itCode;
    }
    if (itCode != storedCode.end()){
        itCode->second = (storedCode.end()-1)->second;
        storedCode.pop_back();
    }

    delete[] temporaryFreqArray;
}

Fano::~Fano() {
    storedUsages.clear();
    storedCode.clear();
}

bool Fano::isEfficiency(){
    if (storedCode.size() != 256) return true;

    for (auto & i : storedCode){
        if (i.second.size() != 8) return true;
    }

    return false;
}

vector< pair<int, int> >::iterator findVectorIt(vector<pair<int, int>>* vec, int value){
    vector< pair<int, int> >::iterator iterator;
    iterator = vec->begin();
    while (iterator != vec->end()){
        if (iterator->first == value) break;
        else iterator++;
    }
    return iterator;
}

void Fano::generateInefficient(std::istream &input, std::ostream &out){
    char ch = 0;
    out.write(&ch, 1);
    while (input.read(&ch, 1)) out.write(&ch, 1);
}

void writeInt(std::ostream& file, char value, string& str){
    // Записали значение
    file.write(&value, 1);

    // Подсчет сколько надо выделить байт
    char size = str.size() / 8;
    if (str.size() % 8 > 0) size++;

    // Записали количество байт сколько надо считать
    file.write(&size, 1);

    // Запись
    for (int i = 0; i < str.length(); i++){
        // Записать концовку
        if (str.length() < 8){
            string temp(str, i, str.length());

            // ПОдсчет количестве нулей в начале
            char k = 0;
            while (temp[k] == '0') k++;
            // Запись количестве нулей в начале
            file.write(&k, 1);

            // Запись string в виде числа
            k = stoi(temp, nullptr, 2);
            file.write(&k, 1);

            break;
        }
        string temp(str, i, 8);

        // Подсчет количестве нулей в начале
        char k = 0;
        while (temp[k] == '0') k++;
        // Запись количестве нулей в начале
        file.write(&k, 1);

        // Запись string в виде числа
        k = stoi(temp, nullptr, 2);
        file.write(&k, 1);

        i = i + 7;
    }
}

void Fano::writeKeys(std::ostream &out) {
    char efficiency = 1;
    out.write(&efficiency, 1);
    char size = storedCode.size();
    out.write(&size, 1);

    // Данные записываются: [символ][кол-во байт которые надо считать] ||[количество нулей в начале][HEX значения]||
    // Если значение начинается с 0, то количество нулей до первой единицы записывается во вторые []
    //
    // Знаком || помечаются границы послежовательности для записи двоичного представления
    //
    // '0100', HEX представление 4 = '100', но из этого не построится дерево.
    // То есть запишется: [символ] ||[01][04] ||
    //
    // Если значение начинается с 1, то ничего не трубется.
    // То есть запишется: [символ] ||[00][значение]||

    for (int i = 0; i < storedCode.size(); i++){

        writeInt(out, storedCode.at(i).first, storedCode.at(i).second);

        std::cout << std::dec << i << " ";
        if ((i % 100 == 0) && (i > 100)){
            std::cout << std::endl;
        }
    }
}

void Fano::generateArchived(std::istream &input, std::ostream &out) {

    Log::v("Scanning and writing... ");

    char ch = 100;
    int posToEndSize = out.tellp();
    int size;
    out.write(&ch, 1);

    string stream;
    unsigned long i = 0;
    unsigned long j = 0;

    while(input.read(&ch, 1)){
        if (stream.length() >= 8){
            if ((j % 50000 == 0) && (j > 0)){
                Log::v("Writed to export file " + std::to_string(int(i / 1000)) + "k bytes.");
            }
            j++;
            string k(stream, 0, 8);
            char outCh = stoi(k, nullptr, 2);
            out.write(&outCh, 1);
            stream.erase(0, 8);
        }
        if ((i % 1000000 == 0) && (i > 0)){
            Log::v("Scanned from imported file " + std::to_string(int(i / 1000000)) + "kk bytes.");
        }
        auto it = storedCode.begin();
        while ((it != storedCode.end()) && (it->first != ch)) ++it;
        if (it == storedCode.end()){
            throw Exception("Not found relation between [streamFile >> ch] and [storedCode]");
        }
        stream += it->second;
        i++;
    }
    size = stream.length() % 8;
    while (stream.length() > 0){
        if ((j % 50000 == 0) && (j > 0)){
            Log::v("Writed to export file " + to_string(int(i / 1000)) + "k bytes.");
        }
        if (stream.length() < 8){
            char outCh = stoi(stream, nullptr, 2);
            out.write(&outCh, 1);
            break;
        }
        string k(stream, 0, 8);
        char outCh = stoi(k, nullptr, 2);
        out.write(&outCh, 1);
        stream.erase(0, 8);
        j++;
    }

    ch = size;
    out.seekp(posToEndSize);
    out.write(&ch, 1);
    /*char ch;
    while(streamFile.read(&ch, 1)){
        if ((i % 1000000 == 0) && (i > 0) && (v)){
            std::cout << "Scanned from imported file " << std::dec << i / 1000000 << "kk bytes." << std::endl;
        }
        auto it = storedCode.begin();
        while ((it != storedCode.end()) && (it->first != ch)) ++it;

        if (it == storedCode.end()){
            std::cout << "Fano.cpp::generateArchived(const string &pathToFile)::127::19 | Not found relation between [streamFile >> ch] and [storedCode]";
            exit(4);
        }

        stream += it->second;
        i++;
    }

    char size = stream.size() % 8;
    archiveFile.write(&size, 1);

    i = 0;
    while (stream.length() > 0){
        if ((i % 50000 == 0) && (i > 0) && (v)){
            std::cout << "Writed to export file " << std::dec << i / 1000 << "k bytes." << std::endl;
        }
        i++;
        if (stream.length() > 8){
            string k(stream, 0, 8);
            char outCh = stoi(k, nullptr, 2);
            archiveFile.write(&outCh, 1);
            stream.erase(0, 8);
            continue;
        }

        while (stream.length() != 8) stream.push_back('0');
        char outCh = stoi(stream, nullptr, 2);
        archiveFile.write(&outCh, 1);
        break;
    }*/
}