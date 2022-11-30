#include "FanoCodesBinary.h"
#include "../utils/Log.h"
#include "Tree.h"
#include "../binary/BinaryIO.h"

#include <stack>

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

std::vector< std::pair<unsigned char, std::vector<bool>>>
        FanoCodesBinary::makeFromFile(std::ifstream &input) {
    size_t bytesFrequency[256] = {0};

    char ch;
    while(input.read(&ch, 1)){
        bytesFrequency[(unsigned char)(ch)]++;
    }

    auto tempArr = mapToSortedVector(bytesFrequency, 256);
    removeZeroUsageBytes(tempArr);

    Node head = Node(vector<bool>());

    makeTree(tempArr.cbegin().base(), tempArr.size(), &head);
    tempArr.clear();

    auto result = mapToBytesArray(&head);
    return result;
}

void FanoCodesBinary::writeToFile(std::ofstream &out, vector<pair<unsigned char, vector<bool>>> &codes) {
    char size = codes.size();
    Log::d("stored codes size " + to_string(codes.size()));
    out.write(&size, 1);

    int inBytePos = 7; //позиция внутри tempByte (будем идти со старшего бита к младшему)
    char tempByte = 0; //байт для записи
    for (auto &p : codes){
        //пишем байт чанка
        BinaryIO::writeByte(out, p.first, tempByte, inBytePos);

        //пишем длинну битов чанка как 1 байтовое число
        BinaryIO::writeByte(out, p.second.size(), tempByte, inBytePos);

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

void addLeafNode(auto &bits, Node &head, char byte){
    Node* thisNode = &head;
    for (size_t i = 0; i < bits.size(); i++){

        if (bits.at(i) == 0){
            if (thisNode->getLeftNode() == nullptr) {
                thisNode->createLeftNode(vector<bool>(bits.begin(), bits.begin() + i));
            }
            thisNode = thisNode->getLeftNode();
        } else {
            if (thisNode->getRightNode() == nullptr) {
                thisNode->createRightNode(vector<bool>(bits.begin(), bits.begin() + i));
            }
            thisNode = thisNode->getRightNode();

        }
    }
    thisNode->setByteAsLeaf(byte);
}

Node FanoCodesBinary::readFromFile(ifstream &source) {
    char ch;
    source.read(&ch, 1);
    //чанк - байт и последовательность битов, соответствующая ему
    size_t chunkCount = ch;
    if (ch == 0){
        chunkCount = 256;//костыль для количества чанков 256 (max char = 255)
    }

    Node head = Node(vector<bool>());

    int chPos = 7; //позиция внутри байта чтения
    size_t readedChunks = 0; //сколько чанков сейчас прочитано

    source.read(&ch, 1); //начали читать

    while (readedChunks < chunkCount) {
        //читаем байт эквивалентный тем, что были в несжатом файле
        unsigned char chunkByte = BinaryIO::readByte(source, chPos, ch);

        //читаем длину битовой последовательности
        unsigned char bitsLen = BinaryIO::readByte(source, chPos, ch);

        auto bits = vector<bool>();

        while (bits.size() < bitsLen) {
            if (chPos == 0) {
                bits.push_back(ch & (1 << chPos));
                source.read(&ch, 1);
                chPos = 7;
            } else {
                bits.push_back(ch & (1 << chPos));
                chPos--;
            }
        }

        addLeafNode(bits, head, chunkByte);

        readedChunks++;
    }

    return head;
}
