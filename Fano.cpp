#include "Fano.h"
#include "utils/Log.h"
#include "utils/Exception.h"

#include <map>
#include <sstream>
#include <vector>
#include <stack>
#include <stdlib.h>
#include <bitset>

using namespace std;

// https://iq.opengenus.org/sorting-vector-in-cpp/
bool operator<(pair<unsigned char, size_t> &a, pair<unsigned char, size_t> &b){
    return a.second > b.second;
}

// [Создание дерева]
// Функция с рекурсией, что разделяет массив на две части так, что разность сумм двух частей была минимальна.
// Рекурсия состоит в том, что необходимо разделить массив неизвестного размера, поэтому функция разделяет массив и подмассивы
// до того момента, как размер новой части массива не станет равна единице (всего один элемент в массиве).
//
// Для перехода на следующий часть в конце алгоритма вызывается эта же функция, но для левого подмассива вызывается прошлый
// указатель на начало массива-родителя, а для правого вызывается указатель на место разрыва массива-родителя.
// Для каждого случая так же вызывается размер массива.
void makeTree(const pair<unsigned char, size_t>* array, size_t arraySize, Node* node){
    // [Конец рекурсии]
    // Создание листьев, если у части массива остался 1 элемент
    if (arraySize == 1){
        node->setByteAsLeaf(array[0].first); // Ввод получившегося символа в лист дерева
        return;
    }

    size_t sumLeft = 0, sumRight = 0, midPos = 0; //Для поиска левой и правой сумм

    for (int i = 0; i < arraySize; i++) {
        sumRight += array[i].second;
    }

    long double balanceCoefficient = (long double)(array[0].second) / array[arraySize-1].second;

    while (sumLeft * balanceCoefficient < sumRight) {
        sumRight -= array[midPos].second;
        sumLeft += array[midPos].second;
        midPos++;
    }

    if (midPos == arraySize) {
        midPos--;
    }

    // [Рекурсия]
    // Для построения дерева с весами '0' или '1', при прохождении влево добавляется '0', при прохождении направо '1'
    // Для левого массива, копирование битов с прошлого нода.
    // Для правого массива, копирование битов с прошлого нода.
    // В качестве аргументов в createLeft(...) входит returnStr() для записывания '0' или '1' к прошлому значению битов.
    auto nextBits = vector<bool>(node->getBits());

    nextBits.push_back(0);
    node->createLeftNode(nextBits);

    nextBits.at(nextBits.size() - 1) = 1;
    node->createRightNode(nextBits);

    makeTree(array + midPos, arraySize - midPos, node->getRightNode());
    makeTree(array, midPos, node->getLeftNode());
}

vector<pair<unsigned char, size_t>> mapToSortedVector(size_t *arr, size_t size){
    auto tempArr = vector<pair<unsigned char, size_t>>();

    for (size_t i = 0; i < size; i++) {
        tempArr.emplace_back(make_pair(i, arr[i]));
    }
    sort(tempArr.begin(), tempArr.end());
    return tempArr;
}

std::vector< std::pair<unsigned char, std::vector<bool>>>
        mapToBytesArray(Node *head){
    auto mapped = std::vector< std::pair<unsigned char, std::vector<bool>>>();
    stack<Node*> stackNodes = stack<Node*>();
    stackNodes.push(head);

    while (!stackNodes.empty()){
        auto cur = stackNodes.top();
        if (cur->isLeaf()){
            mapped.emplace_back(cur->getByte(), cur->getBits());
            stackNodes.pop();
        } else {
            stackNodes.pop();
            stackNodes.push(cur->getRightNode());
            stackNodes.push(cur->getLeftNode());
        }
    }

    return mapped;
}

void removeZeroUsageBytes(vector<pair<unsigned char, size_t>> &vec){
    for (auto it = vec.begin(); it < vec.end();) {
        if (it->second == 0) {
            it = vec.erase(it);
        } else {
            ++it;
        }
    }
}

Fano::Fano(std::istream &input) {
    size_t bytesFrequency[256] = {0};

    Log::d("Read bytes frequency");

    char ch;
    while(input.read(&ch, 1)){
        bytesFrequency[(unsigned char)(ch)]++;
    }

    Log::d("Map to sorted vector");
    auto tempArr = mapToSortedVector(bytesFrequency, 256);
    removeZeroUsageBytes(tempArr);

    Node head = Node(vector<bool>());

    /*Log::d("arr size " + to_string(tempArr.size()));
    auto *array = tempArr.cbegin().base();
    for (size_t i = 0; i < tempArr.size(); i++) {
        Log::d(to_string(array[i].first) + "|" + to_string(array[i].second));
    }*/

    Log::d("Making tree");
    makeTree(tempArr.cbegin().base(), tempArr.size(), &head);
    tempArr.clear();

    storedCode = mapToBytesArray(&head);
}

Fano::~Fano() {
    storedCode.clear();
}

void writeByte(std::ostream &out, char byte, char &usedByte, int &inBytePos){
    int inNumPos = 7;
    while (inNumPos >= 0) {
        while (inBytePos >= 0 && inNumPos >= 0) {
            unsigned char bit = (byte & (1 << inNumPos)); // взяли бит
            int seek = inBytePos - inNumPos; //смотрим куда двигать бит, чтобы занести в tempByte
            if (seek < 0) {
                usedByte |= bit >> -seek;
            } else {
                usedByte |= bit << seek;
            }
            inBytePos--;
            inNumPos--;
        }
        if (inBytePos < 0) {
            out.write(&usedByte, 1);
            usedByte = 0;
            inBytePos = 7;
        }
    }
}

void Fano::writeKeys(std::ostream &out) {
    char size = storedCode.size();
    Log::d("stored codes size " + to_string(storedCode.size()));
    out.write(&size, 1);

    int inBytePos = 7; //позиция внутри tempByte (будем идти со старшего бита к младшему)
    char tempByte = 0; //байт для записи
    for (auto &p : storedCode){
        //пишем байт чанка
        writeByte(out, p.first, tempByte, inBytePos);

        //пишем длинну битов чанка как 1 байтовое число
        writeByte(out, p.second.size(), tempByte, inBytePos);

        //пишем биты
        for (bool b : p.second) {
            tempByte |= (b << inBytePos);
            if (inBytePos == 0) {
                out.write(&tempByte, 1);
                tempByte = 0;
                inBytePos = 7;
            } else {
                inBytePos--;
            }
        }
    }
    if (tempByte != 0) { //дозаписываем остаток, который мог не записаться в цикле
        out.write(&tempByte, 1);
    }
}

void Fano::generateArchived(std::istream &input, std::ostream &out) {
    Log::v("Scanning and writing... ");

    auto codes = map(storedCode.begin(), storedCode.end());

    char ch = 0; //сначала читаем им, потом сразу же в него и пишем

    int inBytePos = 7; //позиция внутри tempByte (будем идти со старшего бита к младшему)
    char tempByte = 0; //байт для записи

    Log::d("body");

    auto chars = vector<char>(1000000);

    while(input.read(&ch, 1)){
        auto &bits = codes[ch];
        chars.push_back(ch);
        //Log::d("---byte " + to_string((unsigned char ) ch));
        //пишем биты
        int pos = 0;
        for (bool b : bits) {
            tempByte |= (b << inBytePos);
            if (inBytePos == 0) {
                //Log::d((stringstream() << bitset<8>(tempByte)).str() + " from " + to_string(pos));
                out.write(&tempByte, 1);
                tempByte = 0;
                inBytePos = 7;
            } else {
                inBytePos--;
            }
            pos++;
        }
    }

    if (inBytePos != 7) {
        out.write(&tempByte, 1); //записали хвост
        Log::d("tail " + (stringstream() << bitset<8>(tempByte)).str() + " from " + to_string((unsigned char)tempByte));
    }
    tempByte = inBytePos;
    Log::d("tail size " + (stringstream() << bitset<8>(tempByte)).str() + " from " + to_string((unsigned char)tempByte));
    out.write(&tempByte, 1);//последний байт всегда показывает длину "хвоста предпоследнего байта"
}